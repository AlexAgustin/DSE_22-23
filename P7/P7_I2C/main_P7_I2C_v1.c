/*
Proyecta texto en la pantalla y queda a la espera de que se pulse S3 (encuesta).
Una vez pulsado, proyecta un nuevo texto en pantalla y queda a la espera de que se pulse 
S4 para inicializar el cronometro (encuesta de S4).
Una vez pulsado, se inicializa el cronometro y se muestra tanto por pantalla como en los leds el 
tiempo transcurrido (led D3 -> ms, led D5 -> seg, led D9 -> min). 
Tambien se mostrara la media de las muestras tomadas de la potencia 
(hay que cambiar el codigo para ver la temperatura/coordenada X/coordenada Y ).
Ademas, se mostrara duty0. 
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
    unsigned char dirI2C;
    
    inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
    
    Init_LCD(); //Inicializacion del LCD
    
    inic_pulsadores(); // inicializacion pulsadores
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_1, 0); //Copiar a memoria RAM la primera linea (mensaje 1)
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_2, 1); //Copiar a memoria RAM la segunda linea (mensaje 2)
    
    line_1(); //nos posicionamos en la primera linea
    puts_lcd((unsigned char*) Ventana_LCD[0], 16); // Llevar al LCD la primera linea, desde RAM
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
    while(PORTDbits.RD6); //Esperar a que se pulse S3 (RD6)
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_3, 0); //Copiar a memoria RAM la primera linea (mensaje 3)
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_4, 1); //Copiar a memoria RAM la segunda linea (mensaje 4)
    
    line_1(); //nos posicionamos en la primera linea
    puts_lcd((unsigned char*) Ventana_LCD[0], 16); // Llevar al LCD la primera linea, desde RAM
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
    while(PORTDbits.RD13); //Esperar a que se pulse S4 (RD13)
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_5, 0); //Copiar a memoria RAM la primera linea (mensaje 5)
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_6, 1); //Copiar a memoria RAM la segunda linea (mensaje 6)

    line_1(); //nos posicionamos en la primera linea
    puts_lcd((unsigned char*) Ventana_LCD[0], 16); // Llevar al LCD la primera linea, desde RAM

    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
    inic_crono();   // Inicializar cronometro
    inic_CN();      // Inicializar modulo CN
    inic_leds();	// Inicializacion leds: sentido y valor inicial.
    inic_Timer7();  // Inicializar el temporizador T7
    inic_UART2();   // Inicializar modulo UART2
    U2TXREG = 'Z';  // Asignacion de un primer caracter para que UART2 TX empiece a interrumpir
    inic_ADC1();    //Inicializar el modulo ADC1
    inic_PWM(); // Inicializar las variables requeridas para la gestion de PWM
    inic_Timer2_PWM();  //Inicializar el temporizador T2
    InitI2C_1();
    
    if (detectar_direccion (&dirI2C)) {
        LATAbits.LATA7=1; // Encender led D10
        while(1); //Espera infinita
    }
    Nop();
    Nop();
    Nop();
    cambiar_direccion (dirI2C, newdirsI2C);
    
    dirI2C=newdirsI2C;
    
    
    if (inic_medicion_dis(dirI2C)){ // Puesta en marcha de una nueva medicion
       LATAbits.LATA3=1; //activar led 6
       while(1);
    }
    inic_Timer6();  // Inicializar el temporizador T6 
    //comienzo_muestreo();    //Comenzar con el muestro de las senhales analogicas
    
    //------------------------A partir de ahora refresco distribuido-------------------------//
    inic_Timer5(); // Inicializar el temporizador T5
    inic_Timer3(); // Inicializar el temporizador T3
 
    
    while(1) { // bucle infinito
        cronometro(); // ejecucion del cronometro
        if (flag_ADC)   //Una vez se han recogido todas las muestras necesarias
            tratar_valorADC1(); // Calcular la media de las muestras tomadas y visualizar la informacion pertinente
        if (flag_Duty_LCD!=0) // Cuando se actualiza el valor de duty0 y se pone a 1 el flag correspondiente (flag_Duty_LCD) ...
            visualizar_Duty(); // se guarda el valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
        if(flag_dis) //Si se puede leer la medicion de la distancia...
            gestion_dis(dirI2C);  //Gestionar la medicion de la distancia
    }
    
	return (0);
}
	





