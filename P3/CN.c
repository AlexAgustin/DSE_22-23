/*
 Funciones relacionadas con el modulo CN: inicializacion del modulo
 y rutina de atencion.

Autores: Alex y Amanda
Fecha: Febrero 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers.h"
int cont4=0, cont5=0, cont6=0;
// Funcion para inicializar el modulo CN
//==================
void inic_CN()
{
  	CNEN1bits.CN15IE = 1;	// habilitacion de la interrupcion del pin CN15
                            // que es al que esta conectado el pulsador S3
    
    CNEN2bits.CN16IE = 1;	// habilitacion de la interrupcion del pin CN16
                            // que es al que esta conectado el pulsador S6
    
    
	IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
	IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}


// RUTINA DE ATENCION A LA INTERRUPCION DE CN
//==============================
void _ISR_NO_PSV _CNInterrupt()
{
    if(!PORTDbits.RD6)     //pulsador S3
	{
        // se ha pulsado S3
		//puesta en marcha y detencion del crono
        T7CONbits.TON = !T7CONbits.TON;
	}
    if(!PORTDbits.RD7)     //pulsador S6
	{
        // se ha pulsado S6
		//reset del crono
        inicializar_crono = 1;
        LATA=LATA & 0xff00; 	// Apagar los leds
        cont6 ++; //se suma uno al contador de interrupciones recibidas (S6)
	}
	IFS1bits.CNIF = 0;	//se marca que la interrupcion ha sido atendida	
}

