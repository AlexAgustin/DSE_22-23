/*
Contiene las funciones de inicializacion y tratamiento de leds y pulsadores.

Autores: Alex y Amanda
Fecha: mayo 2023
*/


#include "p24HJ256GP610A.h"
#include "commons.h"

// Funcion para inicializar los pulsadores
//==================
void inic_pulsadores ()
{
	// Definir pines AN16 - AN31 como pines digitales para evitar problemas
	AD1PCFGH = 0xffff;
       /* Bastaria con definir como digital el pin AN23 que es el que corresponde 
	a RA7 (S5 y D10): AD1PCFGH = 0b 0000 0000 1000 0000 = 0x0080
	Si se deja como pin analogico da problemas, ya que detecta que
	el pulsador S5 esta siempre pulsado aunque no sea asi
	*/
   	
	// Definir los pines de los pulsadores S3, S4, S5, S6 y el boton central del Joystick como entrada
	// S6(RD7),  S4(RD13), S3 (RD6), S5 (RA7), boton central del Joystick (RD14)
    TRISDbits.TRISD6 = 1;   // S3
    TRISDbits.TRISD13 = 1;  // S4
    TRISDbits.TRISD7 = 1;   // S6
    TRISAbits.TRISA7 = 1;   // S5
    TRISDbits.TRISD14 = 1;  // boton central del Joystick
}

// Funcion para inicializar los leds
//==================
void inic_leds()
{
  TRISA = 0xff00; // Definir como salidas los 8 pines conectados a leds: RA7-RA0		  // El resto como entradas: 1 = In; 0 = Out
  Nop(); 
  Nop();
  LATA=LATA & 0xff00; 	// Apagar los leds	
}
