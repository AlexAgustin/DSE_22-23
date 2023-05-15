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


int main()
{
    
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz

	inic_Timer7();	// Inicializacion T7 con un periodo de 10 milisegundos.

    unsigned long cont = 0;
	while(1) {
        cont ++;
        if (IFS3bits.T7IF==1){
            Nop();
            Nop();
            cont=0;
            IFS3bits.T7IF=0;
        }
    } //bucle infinito para la ejecucion del cronometro
	
	return (0);
}
	





