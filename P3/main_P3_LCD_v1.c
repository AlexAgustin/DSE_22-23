/*
Esperar una cantidad determinada de milisegundos (ms) y microsegundos (us).
Se encienden LEDs para indicar que se ha introducido un valor de espera superior a lo contemplado:
 * LED3: cantidad de ms a esperar superior a lo contemplado
 * LED5: cantidad de us a esperar superior a lo contemplado
Autores: Alex Agustin y Amanda Sin
Fecha: 09/02
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


int main()
{
    inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
    
	
    
    Init_LCD(); //Inicializacion del LCD
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_1, 0); //Copiar a memoria RAM la primera linea
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_2, 1); //Copiar a memoria RAM la segunda linea
    
    line_1(); //nos posicionamos en la primera linea
    puts_lcd((unsigned char*) Ventana_LCD[0], 16); // Llevar al LCD la primera linea, desde RAM
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
     while(PORTDbits.RD6);
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_3, 0); //Copiar a memoria RAM la primera linea
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_4, 1); //Copiar a memoria RAM la segunda linea
    
    line_1(); //nos posicionamos en la primera linea
    puts_lcd((unsigned char*) Ventana_LCD[0], 16); // Llevar al LCD la primera linea, desde RAM
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
    while(PORTDbits.RD13);
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_6, 1); //Copiar a memoria RAM la segunda linea
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
    
    inic_crono();
    inic_CN();
    inic_Timer7();
    inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
    while(1) cronometro();
    
	return (0);
}
	





