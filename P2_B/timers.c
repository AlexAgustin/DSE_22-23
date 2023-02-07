#include "p24HJ256GP610A.h"
#include "commons.h"

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
    Nop();
    Nop();
    
    T7CONbits.TON = 1;	// puesta en marcha del timer

}	

unsigned int mili,deci,seg,dec,min;
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

void delay_10ms()	// detecta que el timer ha llegado a 10 milisegundos
{
	
    while (!IFS3bits.T7IF);	// encuesta a la activacion del flag
    IFS3bits.T7IF=0;//flag a 0
 
}

void cronometro()	
// control del tiempo: espera 10 ms y luego actualiza
{
    delay_10ms();	// espera a que pasen 10 milisegundos
  // actualiza las variables del cronometro y modifica los leds segun corresponda
    mili +=10; //se suman 10 milesimas de segundo
    if (mili==100){ //cada 100 milesimas de segundo
        deci+=1; //se suma una decima
        mili=0; //reset milesimas
        LATAbits.LATA0=!LATAbits.LATA0; //conmutar LED3
        if (deci==10){ //cada 10 decimas de segundo
            seg+=1; //se suma 1 seg
            deci=0; //reset decimas
            LATAbits.LATA2=!LATAbits.LATA2; //comuntar LED5
            if (seg==10){ //cada vez que pasen 10 segundos
                dec+=1; //se suma una decena de segundo
                seg=0; //reset segundos
                LATAbits.LATA4=!LATAbits.LATA4;
                if (dec==6){ //cada vez que pasen 6 decenas de segundo
                    min+=1; //se suma 1 minuto
                    dec=0; //reset decenas
                    LATAbits.LATA6=!LATAbits.LATA6;
                }
            }
            
        }
    }
    
    
    
    /*if((seg%10)==0){
        LATAbits.LATA4=!LATAbits.LATA4;
    }*/
}

