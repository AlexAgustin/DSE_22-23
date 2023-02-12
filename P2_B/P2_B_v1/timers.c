/*
Funciones relacionadas con el modulo T7 y el cronometro.

Autores: Alex y Amanda
Fecha: Febrero 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
int inicializar_crono = 0;

void cronometro();
void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  50000-1 ;	// Periodo del timer
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan 400.000 ciclos.
    T7CONbits.TCKPS = 1;	// escala del prescaler 01
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC3bits.T7IE = 1;      // habilitacion de la interrupcion general de T7
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
    
    T7CONbits.TON = 1;	// el timer empieza en estado apagado
}	
unsigned int mili,deci,seg,dec,min;
void _ISR_NO_PSV _T7Interrupt()
{
    mili +=10; //se suman 10 milesimas de segundo
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
}


void inic_crono()	
// inicializacion de las variables del cronometro: 
// milesimas de segundo (mili), decimas de segundo (deci), segundos (seg), decenas de segundos (dec), minutos (min)
{
	mili=0;
    deci=0;
    seg=0;
    dec=0;
    min=0;
}


void cronometro()	
// control del tiempo: espera 10 ms y luego actualiza
// inicializar cronometro: si el flag inicializar_crono esta activado, inicializa el cronometro
{
    if(inicializar_crono)
    {
        //el flag inicializar_crono esta activado
        inic_crono(); //inicializa el cronometro
        inicializar_crono = 0; //puesta a 0 del flag inicializar_crono
    }

  // actualiza las variables del cronometro y modifica los leds segun corresponda
    if (mili>=100){ //cada 100 milesimas de segundo
        deci+=1; //se suma una decima
        mili-=100; //reset milesimas
        LATAbits.LATA0=!LATAbits.LATA0; //conmutar LED D3

        if (deci>=10){ //cada 10 decimas de segundo
            seg+=1; //se suma 1 seg
            deci-=10; //reset decimas
            LATAbits.LATA2=!LATAbits.LATA2; //conmuntar LED D5

            if (seg>=10){ //cada vez que pasen 10 segundos
                dec+=1; //se suma una decena de segundo
                seg-=10; //reset segundos
                LATAbits.LATA4=!LATAbits.LATA4; //conmuntar LED D7

                if (dec>=6){ //cada vez que pasen 6 decenas de segundo
                    min+=1; //se suma 1 minuto
                    dec-=6; //reset decenas
                    LATAbits.LATA6=!LATAbits.LATA6; //conmuntar LED D9
                }
            }
            
        }
    }
}

