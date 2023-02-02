/*
 El programa se queda a la espera de que se pulse S3. 
 Se marca que el programa esta a la espera con el LED D9 activado. 
 Una vez pulsado S3, se apaga el LED D9 y se enciende el LED D8.
 A partir de este momento y no antes, se tratan las siguientes interrupciones:
 - si se pulsa S4, se inverte el estado del LED D3
 - si se pulsa S5, se inverte el estado del LED D5
 - si se pulsa S6, se inverte el estado del LED D7

Autores: Alex y Amanda
Fecha: Febrero 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "GPIO.h"
#include "CN.h"



// programa principal
int main()
{
 inic_leds();        
 inic_pulsadores();          

 LATAbits.LATA6 = 1;  //led D9 (RA6) encendido

 while (PORTDbits.RD6); //esperar a que se pulse el pulsador S3 (por encuesta)
  
 LATAbits.LATA6 = 0;  //led D9 (RA6) apagado
 LATAbits.LATA5 = 1;  //led D8 (RA5) encendido

 inic_CN(); // habilita las interrupciones de los pulsadores S4, S5 y S6
 
 while(1); // el programa funcionará únicamente por interrupciones
}
