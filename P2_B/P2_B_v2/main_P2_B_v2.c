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



int main()
{
    
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
	
	inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
	Nop();
	Nop();
	// Espera de milisegundos 
	delay_ms(5);
	delay_ms(25);
	delay_ms(150);
	delay_ms(425); // se sale del rango maximo
    
	Nop();
	Nop();
	// Espera de microsegundos 
	delay_us(400);
	delay_us(1000);
	delay_us(20000);
	delay_us(420000); //se sale del rango maximo

	return (0);
}
	





