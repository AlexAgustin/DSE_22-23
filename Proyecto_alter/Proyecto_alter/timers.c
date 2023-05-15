#include "p24HJ256GP610A.h"
#include "commons.h"

void inic_Timer7 ()
{
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  12500 -1 ;	// Periodo del timer
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan 400.000 ciclos.
    T7CONbits.TCKPS = 2;	// escala del prescaler 01
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC3bits.T7IE = 0;      // deshabilitar la interrupcion general de T7
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
    
    T7CONbits.TON = 1;	// puesta en marcha del timer
}	
