/*
Proyecta texto en la pantalla y queda a la espera de que se pulse S3 (encuesta).
Una vez pulsado, proyecta un nuevo texto en pantalla y queda a la espera de que se pulse 
S4 para inicializar el cronometro (encuesta de S4).
Una vez pulsado, se inicializa el cronometro y se muestra tanto por pantalla como en los leds el 
tiempo transcurrido (led D3 -> ms, led D5 -> seg, led D9 -> min).
A partir de este punto se permite la interrupcion de los pulsadores S3 y S6:
* pulsador S3 -> parar/reanudar cronometro
* pulsador S6 -> inicializar cronometro (puesta a 0)
A partir de este momento se da refresco distribuido para actualizar la informacion en la pantalla LCD.
Ademas, visualizamos en el ordenador (a traves del emisor de UART2) la informacion que mandamos a la LCD.
6 teclas presionadas en el PC tienen efecto sobre el cronometro gracias al receptor del modulo UART2: 
 * I e i, inicializar el crono; 
 * P y p, parar el crono; 
 * C y c, poner en marcha el crono. 
 * El resto de caracteres no afectaran al cronometro.
Se mostrara la tecla presionada en la ultima posicion de la segunda linea tanto en el modulo LCD como
en la pantalla del PC.
Autores: Alex Agustin y Amanda Sin
Fecha: Febrero 2023
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


int main()
{
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
    
    inic_crono(); // Inicializar cronometro
    inic_CN(); // Inicializar modulo CN
    inic_leds();	// Inicializacion leds: sentido y valor inicial.
    inic_Timer7(); // Inicializar modulo T7
    inic_UART2(); // Inicializar modulo UART2
    
    
    //------------------------A partir de ahora refresco distribuido-------------------------//
    inic_Timer5(); // Inicializacion del temporizador T5
    U2TXREG = 'Z'; // Asignacion de un primer caracter para que UART2 TX empiece a interrumpir
    
    inic_ADC1();
    comienzo_muestreo();
    
    while(1) {
        cronometro(); //Bucle infinito para la ejecucion del cronometro
        if (flag_ADC)recoger_valorADC1();
    }
    
	return (0);
}
	





