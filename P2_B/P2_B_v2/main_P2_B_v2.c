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



int main()
{
    
	inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
	
	inic_leds();	// Inicializacion leds: sentido y valor inicial.
    
	Nop();
    Nop();
    delay_ms(5);
    delay_ms(25);
    delay_ms(150);
    delay_ms(425); // se sale del rango maximo
    
    Nop();
    Nop();
    delay_us(400);
    delay_us(1000);
    delay_us(20000);
    delay_us(420000); //se sale del rango maximo

	return (0);
}
	





