/*
 Funciones relacionadas con el modulo CN: inicializacion del modulo
 y rutina de atencion.

Autores: Alex y Amanda
Fecha: Febrero 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
int cont4=0, cont5=0, cont6=0;
// Funcion para inicializar el modulo CN
//==================
void inic_CN()
{
  	CNEN2bits.CN19IE = 1;	// habilitacion de la interrupcion del pin CN19
                            // que es al que esta conectado el pulsador S4
    
 	CNEN2bits.CN23IE = 1;	// habilitacion de la interrupcion del pin CN23
                            // que es al que esta conectado el pulsador S5
    
 	CNEN2bits.CN16IE = 1;	// habilitacion de la interrupcion del pin CN16
                            // que es al que esta conectado el pulsador S6
    
	IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
	IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}


// RUTINA DE ATENCION A LA INTERRUPCION DE CN
//==============================
void _ISR_NO_PSV _CNInterrupt()
{
		if (!PORTDbits.RD13)    // pulsador S4
			{
                // se ha pulsado S4
				LATAbits.LATA0 = !LATAbits.LATA0; //conmutar el estado del LED D3
                cont4 ++; //se suma uno al contador de interrupciones recibidas (S4)
			}
        if(!PORTAbits.RA7)     //pulsador S5
			{
                // se ha pulsado S5
				LATAbits.LATA2 = !LATAbits.LATA2; //conmutar el estado del LED D5
                cont5 ++; //se suma uno al contador de interrupciones recibidas (S5)
			}
        if(!PORTDbits.RD7)     //pulsador S6
			{
                // se ha pulsado S6
				LATAbits.LATA4 = !LATAbits.LATA4; //conmutar el estado del LED D7
                cont6 ++; //se suma uno al contador de interrupciones recibidas (S6)
			}
	IFS1bits.CNIF = 0;	//se marca que la interrupcion ha sido atendida	
}

