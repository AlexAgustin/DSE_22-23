/*
Funcion de inicializacion y tratamiento de leds

Autores: Alex y Amanda
Fecha: Febrero 2023
*/


#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_leds()
{
  TRISA = 0xff00; // Definir como salidas los 8 pines conectados a leds: RA7-RA0		  // El resto como entradas: 1 = In; 0 = Out
  Nop();	
  Nop();
  LATA=LATA & 0xff00; 	// Apagar los leds
	
}
      
