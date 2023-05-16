/*
Proyecta texto en la pantalla y queda a la espera de que se pulse S3 (encuesta).
Una vez pulsado, proyecta un nuevo texto en pantalla y queda a la espera de que se pulse 
S4 para inicializar el cronometro (encuesta de S4).
Una vez pulsado, se inicializa el cronometro y se muestra tanto por pantalla como en los leds el 
tiempo transcurrido (led D3 -> ms, led D5 -> seg, led D9 -> min). 
Tambien se mostrara la media de las muestras tomadas de la potencia 
(hay que cambiar el codigo para ver la temperatura/coordenada X/coordenada Y/duty0/duty1 ).
Se detecta la direccion del sensor conectado y se modifica su valor mediante la constante newdirsI2C (commos.h).
Ademas, se mostrara la distacia recogida por el sensor. 
Mencionar que en esta version se gestionan 2 servomotores con un unico temporizador.

A partir de este punto se permite la interrupcion de los pulsadores S3, S4 y S6:
* pulsador S3 -> parar/reanudar cronometro
* pulsador S4 -> cambiar la gestion de duty0 de modo que se obtenga a partir de la potencia o se modifique por UART
* pulsador S6 -> inicializar cronometro (puesta a 0)
A partir de este momento se da refresco distribuido para actualizar la informacion en la pantalla LCD.
Ademas, visualizamos en el ordenador (a traves del emisor de UART2) la informacion que mandamos a la LCD.
10 teclas presionadas en el PC tienen efecto sobre el programa gracias al receptor del modulo UART2: 
 * I e i, inicializar el crono.
 * P y p, parar el crono.
 * C y c, poner en marcha el crono. 
 * R, mover el servo incrementando duty1 (+10) si se respetan los limites (<= DUTY_MAX).
 * L, mover el servo decrementando duty1 (-10) si se respetan los limites (>= DUTY_MIN).
 * r, mover el servo incrementando duty0 (+10) si se respetan los limites (<= DUTY_MAX) y se gestiona duty0 por UART (flag_DUTY == 1).
 * l, mover el servo decrementando duty0 (-10) si se respetan los limites (>= DUTY_MIN) y se gestiona duty0 por UART (flag_DUTY == 1).
 * El resto de caracteres no afectaran al programa.
Se mostrara la tecla presionada en la ultima posicion de la segunda linea tanto en el modulo LCD como
en la pantalla del PC.
Autores: Alex Agustin y Amanda Sin
Fecha: Merzo 2023
*/

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "LCD.h"
#include "memoria.h"
#include "CN.h"
#include "utilidades.h"
#include "UART2_RS232.h"
#include "ADC1.h"
#include "OCPWM.h"
#include "srf08.h"
#include "i2c_funciones.h"

int main()
{
    unsigned long cont = 0;
    unsigned char dirI2C; //Variable que representa la direccion del sensor
    
    inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
    
    Init_LCD(); //Inicializacion del LCD
    
    inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
    inic_pulsadores(); // inicializacion pulsadores
    
    inic_UART2();   // Inicializar modulo UART2
    U2TXREG = 0;  // Asignacion de un primer caracter para que UART2 TX empiece a interrumpir
    
    InitI2C_1();    //Inicializar el modulo I2C
    inic_Timer6_dis();  // Inicializar el temporizador T6 
    
    if (detectar_direccion (&dirI2C)) { //Detectar la nueva direccion del sensor
        //Comportamiento inesperado
        LATAbits.LATA3=1; // Encender led D6
        while(1); //Espera infinita
    }
    
    if (inic_medicion_dis(dirI2C)){ // Puesta en marcha de una nueva medicion
        //Ha ocurrido un error en la puesta en marcha de la medicion
        LATAbits.LATA1=1; // Encender led D4
        while(1); //Espera infinita
    }
    
    inic_Timer5_LCD(); // Inicializar el temporizador T5 //Siempre refresco ditribuido
    
    while(PORTDbits.RD14);
    
    inic_calib();
    inic_Timer4_movservos(); //Inicializar Timer del movimiento
    visualizar_Duty();
    inic_PWM(); // Inicializar las variables requeridas para la gestion de PWM
    inic_Timer8_PWM();  //Inicializar el temporizador T8

    while(PORTDbits.RD13) visualizar_Duty(); //Esperar a que se pulse S4 (RD13)

    flag_calib = 0; //Deshabilitar opcion de calibrado
    
    inic_CN();      // Inicializar modulo CN
    actualizar_Ventana_LCD(); //Actualizar la variable Ventana_LCD
    
    inic_crono();   // Inicializar cronometro
    inic_Timer7_crono();  // Inicializar el temporizador T7
       
    inic_Ruedas();
    inic_OC1(); //Inicializar el modulo OC1
    inic_OC2(); //Inicializar el modulo OC1
    inic_OC3();
    inic_OC4();
    inic_Timer2_OCx();  //Inicializar el temporizador T2

    inic_ADC1();    //Inicializar el modulo ADC1
    inic_Timer3_ADC(); // Inicializar el temporizador T3
    reinic_Timer9_CPU();
    
    while(1) { // bucle infinito
        cont = 0;
        restart_Timer9_CPU();
        cronometro(); // ejecucion del cronometro
        if (flag_ADC)   //Una vez se han recogido todas las muestras necesarias
            tratar_valorADC1(); // Calcular la media de las muestras tomadas y visualizar la informacion pertinente
        if (flag_Duty_LCD!=0) // Cuando se actualiza el valor de duty0 y se pone a 1 el flag correspondiente (flag_Duty_LCD) ...
            visualizar_Duty(); // se guarda el valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
        if (flag_dis) //Si se puede leer la medicion de la distancia...
            gestion_dis(dirI2C);  //Gestionar la medicion de la distancia
        if (flag_posicion_segura)
            posicion_segura(); //Llevar el brazo a una posicion segura
        if (flag_rutina_perro && reached == 5){rutina_perro(); }//dibujar una estrella
        if (flag_exit){
            break;
        }

        while(IFS3bits.T9IF == 0) cont ++;
        stop_Timer9_CPU();
        gestion_cont (cont);
    }
    
	return (0);
}
	





