/*
Cronometro mediante leds.
Control de temporizador por encuesta.
Se utilizan los siguientes leds:
 * LED3 para decimas de segundo
 * LED5 para segundos

Autores: Alex Agustin y Amanda Sin
Fecha: 07/02
*/

#include "p24HJ256GP610A.h"

#include "oscilator.h"
#include "GPIO.h"
#include "timers.h"
#include "commons.h"
#include "CN.h"


int main()
{
    
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
	
	inic_leds();	// Inicializacio³n leds: sentido y valor inicial.
	
	inic_crono();	//Inicializacion variables cronometro.

	inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.
    
    inic_CN();      //Inicializacion de los pulsadores

	while(1) cronometro(); //bucle infinito para la ejecucion del cronometro
	
	return (0);
}
	





