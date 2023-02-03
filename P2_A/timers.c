#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    //PR7 =  50000-1 ;	// Periodo del timer
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan 400.000 ciclos.
   // T7CONbits.TCKPS = 1;	// escala del prescaler 01
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    T7CONbits.TON = 1;	// puesta en marcha del timer
}	

unsigned int mili,deci,seg,min;
void inic_crono()	
// inicializacion de las variables del cronometro
{
	//completad codigo
}

void delay_10ms()	// detecta que el timer ha llegado a 10 milisegundos
{
	//completad codigo
    //while ();	// encuesta continua
 
}

void cronometro()	
// control del tiempo espera 10 ms y luego actualiza
{
    delay_10ms();	// espera a que pasen 10 milisegundos
  // completad codigo
  // actualiza las variables del cronometro y modifica los leds segun corresponda
    
}

