/*
Cronometro mediante leds.
Control de temporizador por interrupcion.
Se utilizan los siguientes leds:
 * LED3 para decimas de segundo
 * LED5 para segundos
 * LED7 para decenas de segundos
 * LED9 para minutos
Control del inicio del temporizador mediante encuesta del pulsador RD13 (S4).
Control del temporizador mediante pulsadores por interrupcion.
Se usan los siguientes botones:
 * RA7 (S5) para parar o continuar con el cronometro
 * RD7 (S6) para reiniciar el cronometro

Autores: Alex Agustin y Amanda Sin
Fecha: 09/02
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
    
    inic_pulsadores();
	
	inic_crono();	//Inicializacion variables cronometro.
    
    while (PORTDbits.RD13);    // esperar al pulsador S4

    inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.
    
    inic_CN();      //Inicializacion de los pulsadores
    
	while(1) cronometro(); //bucle infinito para la ejecucion del cronometro
	
	return (0);
}
	





