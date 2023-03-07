/* Funciones para el modulo ADC1
COMENTARIOS!!!!!
Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include <p24HJ256GP610A.h>
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"


//int flag_CAD =0;

void inic_ADC1 (void)
{
// Inicializacion registro control AD1CON1
AD1CON1 = 0;       // todos los campos a 0

// Salida de 12 bits o 10 bits
//AD1CON1bits.AD12B = 0;  

// Comienzo digitalizacion automatico
// 111=Auto-convert / 010=TMR3 ADC1 y TMR5 ADC2 / 001=INT0 / 000= SAMP 
AD1CON1bits.SSRC= 7;  //Finaliza con el temporizador interno	

// Muestreo simultaneo o secuencial
//AD1CON1bits.SIMSAM = 0; 
             
// Comienzo muestreo automático o por programa (SAMP=1) 		
//AD1CON1bits.ASAM = 0;

                    
// Inicializacion registro control AD1CON2
AD1CON2= 0 ;  // todos los campos a 0


// Inicializacion registro control AD1CON3
AD1CON3=0;    // todos los campos a 0
// Reloj con el que funciona el ADC:  0= reloj CPU; 1= RC erlojua 
//AD1CON3bits.ADRC = 0;  // 
AD1CON3bits.SAMC=31;   // Tiempo muestreo = numero de Tad 
AD1CON3bits.ADCS=3;   // Relacion entre TAD y Tcy TAD = Tcy(ADCS+1)


// Inicializacion registro control AD1CON4
AD1CON4=0; //relacionado con DMA, no lo usaremos


// Inicializacion registro AD1CHS123
AD1CHS123=0;	//seleccion del canal 1,2 y 3


// Inicializacion registro AD1CHS0
AD1CHS0=0; 
AD1CHS0bits.CH0SA=5; // elige la entrada analogica conectada

//AD1CHS0bits.CH0SB = 0;


// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0

AD1CSSH = 0;   // 16-31 
AD1CSSL = 0;   // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales
// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5=0;   // potenciometro
AD1PCFGLbits.PCFG4=0;   // sensor temperatura

// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF=0;    
IEC0bits.AD1IE=0; //trabajaremos por encuesta   
//IPC3bits.AD1IP=xx;    

//AD1CON
AD1CON1bits.ADON=1;  // Habilitar el modulo ADC
}


// comienzo del muestreo por programa
void comienzo_muestreo ()
{
    AD1CON1bits.SAMP=1; // Comienza el muestreo y 31Tad despues comienza la digit
}


// Funcion que recoge el valor del convertidor por encuesta
void recoger_valorADC1 () 
{
    unsigned int ADC_value;
	//comienzo_muestreo();
    if (AD1CON1bits.DONE){ // Encuesta: comprobar si ha terminado la digitalizacion
        ADC_value = ADC1BUF0; //Dejar el valor en ADC_Value
        Nop();
        Nop();
        //flag_CAD=1;
        
        
        if (AD1CHS0bits.CH0SA==5){ // se esta muestreando el potenciometro
            conversion_ADC(&Ventana_LCD[0][pospoten],ADC_value); //escribimos el valor de la potencia en la posicion correspondiente
            AD1CHS0bits.CH0SA=4; // pasamos a muestrear el termometro
        }else{
            if (AD1CHS0bits.CH0SA==4) { // se esta muestreando el termometro
                conversion_ADC(&Ventana_LCD[0][postemp],ADC_value); //escribimos el valor de la temperatura en la posicion correspondiente
                AD1CHS0bits.CH0SA=5; //pasamos a muestrear el potenciometro
            }
        }
        comienzo_muestreo(); // Comienza el muestreo y 31Tad despues comienza la digit
    }
}


