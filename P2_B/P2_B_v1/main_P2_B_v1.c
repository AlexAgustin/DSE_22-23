/*
Cronometro mediante leds.
Control de temporizador por interrupcion.
Se utilizan los siguientes leds:
 * LED D3 para decimas de segundo
 * LED D5 para segundos
 * LED D7 para decenas de segundos
 * LED D9 para minutos
Control del inicio del temporizador mediante encuesta del pulsador S4 (RD13).
Control del temporizador mediante pulsadores por interrupcion.
Se usan los siguientes botones:
 * S5 (RA7) para parar o continuar con el cronometro
 * S6 (RD7) para reiniciar el cronometro

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
	
	inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
    inic_pulsadores(); // Inicializacion pulsadores
	
	inic_crono();	//Inicializacion variables cronometro.
    
    while (PORTDbits.RD13);    // esperar al pulsador S4

    inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.
    
    inic_CN();     // habilita las interrupciones de los pulsadores S5 y S6
    
	while(1) cronometro(); //bucle infinito para la ejecucion del cronometro
	
	return (0);
}
	





