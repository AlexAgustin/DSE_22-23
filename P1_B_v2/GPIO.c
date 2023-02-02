/*
Funciones de inicializacion y tratamiento de pulsadores y leds

Egileak: Alex y Amanda
Data: Febrero 2023
*/


#include "p24HJ256GP610A.h"

void inic_pulsadores ()
{
	// Definir pines AN16 - AN31 como pines digitales para evitar problemas
	AD1PCFGH = 0xffff;
       /* Bastaria con definir como digital el pin AN23 que es el que corresponde 
	a RA7 (S5 y D10): AD1PCFGH = 0b 0000 0000 1000 0000 = 0x0080
	Si se deja como pin analogico da problemas, ya que detecta que
	el pulsador S5 esta siempre pulsado aunque no sea asi
	*/
   	
	// Definir los pines de los pulsadores S3, S4, S5 y S6 como entrada
	// S6(RD7), S5 (RA7),  S4(RD13), S3 (RD6)

	TRISDbits.TRISD7 = 1;
    TRISAbits.TRISA7 = 1;
	TRISDbits.TRISD13 = 1;
    TRISDbits.TRISD6 = 1;
}

void inic_leds()
{
    // Definir los 8 pines del puerto A conectados a leds (RA7-RA0) como salidas
    // El reso como entradas: 1 = In; 0 = Out
	TRISA = 0xff00;		
										
    /*El pin RA7 lo comparten el led D10 y el pulsador S5.
     Hemos puesto su direccion a 0= output. Por lo tanto si queremos usar
     el pulsador S5, habra que cambiarlo.
    */
}
