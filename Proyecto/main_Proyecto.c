/*
Proyecta texto en la pantalla y queda a la espera de que se pulse el boton central del joystick (encuesta).
Mencionar que para actualizar la informacion en la pantalla LCD se da refresco distribuido.
Una vez pulsado, entra en la fase de calibrado donde se pueden ajustar los valores maximo y minimo de cada servo y 
queda a la espera de que se pulse S4 para inicializar el cronometro (encuesta de S4).
Una vez pulsado, se inicializa el cronometro y se muestra tanto por pantalla como en los leds el 
tiempo transcurrido (led D3 -> ms, led D5 -> seg, led D9 -> min). 
Para visualizar mas informacion se habra de hacer scroll hacia abajo (S5) y hacia arriba (S4).
Se mostrara la media de las 8 muestras tomadas de 5 entradas analogicas: potenciometro, temperatura, coordenadas X, Y e Z. 
Se detecta la direccion del sensor conectado y se modifica su valor mediante la constante newdirsI2C (commons.h).
Ademas, se mostrara la distacia recogida por el sensor, la cual condiciona el movimiento de algunos servos. 
Mencionar que en esta version se gestionan 5 servomotores con un unico temporizador (T8), cuyos valores actuales, maximos y minimos
pueden visualizarse.

A partir de este punto se permite la interrupcion de los pulsadores S3, S4, S5 y S6:
* pulsador S3 -> parar/reanudar cronometro
* pulsador S4 -> scroll hacia arriba
* pulsador S5 -> scroll hacia abajo
* pulsador S6 -> inicializar cronometro (puesta a 0)

Ademas, visualizamos en el ordenador (a traves del emisor de UART2) la informacion que mandamos a la LCD.
30 teclas presionadas en el PC tienen efecto sobre el programa gracias al receptor del modulo UART2: 
 * I e i, inicializar el crono.
 * P y p, parar el crono.
 * C y c, poner en marcha el crono. 
 * X y x, cambiar modo de gestion de los servos S1 (duty0), S3 (duty2), S5 (duty4). Flag=0, gestion por entradas analogicas; flag=1, gestion por UART
 * M, para incrementar el duty del servo que se este calibrando.
 * m, para decrementar el duty del servo que se este calibrando.
 * F y f, para fijar el valor del duty que se este calibrando.
 * R, mover el servo S1 incrementando duty0 (+5) si se respetan los limites (<= DUTY_MAX).
 * L, mover el servo S2 incrementando duty1 (+5) si se respetan los limites (<= DUTY_MAX).
 * W, mover el servo S3 incrementando duty2 (+5) si se respetan los limites (<= DUTY_MAX).
 * A, mover el servo S4 incrementando duty3 (+5) si se respetan los limites (<= DUTY_MAX).
 * S, mover el servo S5 incrementando duty4 (+5) si se respetan los limites (<= DUTY_MAX).
 * r, mover el servo S1 decrementando duty0 (-5) si se respetan los limites (>= DUTY_MIN).
 * l, mover el servo S2 decrementando duty1 (-5) si se respetan los limites (>= DUTY_MIN).
 * w, mover el servo S3 decrementando duty2 (-5) si se respetan los limites (>= DUTY_MIN).
 * a, mover el servo S4 decrementando duty3 (-5) si se respetan los limites (>= DUTY_MIN).
 * s, mover el servo S5 decrementando duty4 (-5) si se respetan los limites (>= DUTY_MIN).
 * >, para hacer scroll hacia abajo en la pantalla LCD.
 * <, para hacer scroll hacia arriba en la pantalla LCD.
 * D y d, pera realizar la rutina canina de entrenamiento.
 * Z y z, para llevar el brazo a una posicion segura.
 * Q y q, para terminar el programa.
 * El resto de caracteres no afectaran al programa.

Se mostrara la tecla presionada en la ultima posicion de la linea correspondinte al cronometro tanto en el modulo LCD como
en la pantalla del PC.
La velocidad de las ruedas depende de la media de la coordenada Y.

Servos:
S1 (duty0): puede gestionarse por potenciometro o UART. 
S2 (duty1): puede gestionarse por UART. 
S3 (duty2): puede gestionarse por coordenada X o UART. 
S4 (duty3): puede gestionarse por UART. 
S5 (duty4): puede gestionarse por coordenada Z o UART. 

Autores: Alex Agustin y Amanda Sin
Fecha: Mayo 2023
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
    unsigned long cont = 0; // inicializar contador para medir el uso de la CPU
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

    if(dirI2C != newdirsI2C) // Si la direccion detectada no es la guardada como direccion en el proyecto...
    {
        cambiar_direccion (dirI2C, newdirsI2C); //Cambiar la direccion del sensor
        
        if (detectar_direccion (&dirI2C)) { //Detectar la nueva direccion del sensor
            //Comportamiento inesperado
            LATAbits.LATA3=1; // Encender led D6
            while(1); //Espera infinita
        }
    }

    if (inic_medicion_dis(dirI2C)){ // Puesta en marcha de una nueva medicion
        //Ha ocurrido un error en la puesta en marcha de la medicion
        LATAbits.LATA1=1; // Encender led D4
        while(1); //Espera infinita
    }
    
    inic_Timer5_LCD(); // Inicializar el temporizador T5 //Siempre refresco ditribuido
    
    while(PORTDbits.RD14); //Esperar a que se pulse el boton central del joystick
    
    inic_calib(); //Inicializar las varibles necesarias para la calibracion
    inic_Timer4_movservos(); //Inicializar Timer del movimiento de los servos
    visualizar_Duty(); //Visualizar valores por defecto de calibracion
    inic_PWM(); // Inicializar las variables requeridas para la gestion de PWM
    inic_Timer8_PWM();  //Inicializar el temporizador T8

    while(PORTDbits.RD13) if (flag_Duty_LCD) visualizar_Duty(); //Esperar a que se pulse S4 (RD13) mientras se visualizan los cambios de la calibracion

    flag_calib = 0; //Deshabilitar opcion de calibrado
    
    inic_CN();      // Inicializar modulo CN
    actualizar_Ventana_LCD(); //Actualizar la informacion que muestra la pantalla LCD
    
    inic_crono();   // Inicializar cronometro
    inic_Timer7_crono();  // Inicializar el temporizador T7
       
    inic_Ruedas(); // Gestion de pins para los motores de las ruedas (Enable)
    inic_OC1(); //Inicializar el modulo OC1
    inic_OC2(); //Inicializar el modulo OC2
    inic_OC3();//Inicializar el modulo OC3
    inic_OC4(); //Inicializar el modulo OC4
    inic_Timer2_OCx();  //Inicializar el temporizador T2 (gestion modulos OC)

    inic_ADC1();    //Inicializar el modulo ADC1
    inic_Timer3_ADC(); // Inicializar el temporizador T3 (gestion ADC1)
    reinic_Timer9_CPU(); // Reinicializar modulo T9 (gestion CPU)
    
    while(1) { // bucle infinito
        cont = 0; //Inicializar contador (gestion CPU)
        restart_Timer9_CPU(); // Encender timer T9
        cronometro(); // ejecucion del cronometro

        if (flag_ADC)   //Una vez se han recogido todas las muestras necesarias
            tratar_valorADC1(); // Calcular la media de las muestras tomadas y visualizar la informacion pertinente
        if (flag_Duty_LCD!=0) // Cuando se actualiza el valor de duty0 y se pone a 1 el flag correspondiente (flag_Duty_LCD) ...
            visualizar_Duty(); // se guarda el valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
        if (flag_dis) //Si se puede leer la medicion de la distancia...
            gestion_dis(dirI2C);  //Gestionar la medicion de la distancia
        if (flag_DUTY && flag_posicion_segura) posicion_segura(); //Llevar el brazo a una posicion segura si el flag flag_posicion_segura esta activado y el control de los servos no se esta realizando mediante senhales analogicas 
        if (flag_rutina_perro && reached == 5) //Si el flag esta activado y los servos estan en su posicion objetivo...
            rutina_perro(); // El brazo realiza una rutina canina 
        if (flag_exit){ //Termina el programa
            break;
        }

        while(IFS3bits.T9IF == 0) cont ++; // Obtener conts restantes (20 ms) => medicion del uso de la CPU
        stop_Timer9_CPU();//Parar timer T9
        gestion_cont (cont); // Gestion del valor de cont (calculo del porcentaje de uso de CPU)
    }
    
	return (0);
}
	





