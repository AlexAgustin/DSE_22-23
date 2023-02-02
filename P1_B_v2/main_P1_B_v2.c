/*
 El programa se queda a la espera de que se pulse S3.
 * Tras esto, se tratan las siguientes interrupciones ...
Egileak: Alex y Amanda
Data: Febrero 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "GPIO.h"
#include "CN.h"



int main()
{
 inic_leds();        
 inic_pulsadores(); 
 inic_CN();          

 LATAbits.LATA6 = 1;  //led D9 (RA6) encendido

 while (PORTDbits.RD6); 
  
 LATAbits.LATA6 = 0;  //led D9 (RA6) apagado
 LATAbits.LATA5 = 1;  //led D8 (RA6) encendido

 enable_IE();
 
 while(1); // el programa funcionará únicamente por interrupciones
}
