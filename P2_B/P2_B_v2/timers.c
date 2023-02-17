/* 
Contiene las funciones asociadas al temporizador T9 y 
a las esperas de un determinado numero de milisegundos y microsegundos
*/
#include "p24HJ256GP610A.h"
#include "commons.h"

// inicializacion del timer 9
void inic_Timer9(unsigned long ciclos){
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    if (ciclos < 65535) {
         T9CONbits.TCKPS = 0;	// escala del prescaler 00
         PR9 =  ciclos-1 ;	// Periodo del timer con prescaler 00
    }else{
        if ((ciclos/8) < 65535){
            T9CONbits.TCKPS = 1;	// escala del prescaler 01
            PR9 =  (ciclos/8)-1 ;	// Periodo del timer con prescaler 01
        }else{
            if ((ciclos/64) < 65535){
                T9CONbits.TCKPS = 2;	// escala del prescaler 10
                PR9 =  (ciclos/64)-1 ;	// Periodo del timer con prescaler 10
            }else{
                if ((ciclos/256) < 65535) {
                    T9CONbits.TCKPS = 3;	// escala del prescaler 11
                    PR9 =  (ciclos/256)-1 ;	// Periodo del timer con prescaler 11
                }
            }
        }
    }
    T9CONbits.TCS = 0;	// reloj interno
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    IFS3bits.T9IF = 0;      // Puesta a 0 del flag IF del temporizador 9
    T9CONbits.TON = 1;	// puesta en marcha del timer
}


//Espera un determinado numero de milisegundos
void delay_ms(unsigned int ms){
    unsigned long ciclos = (unsigned long)ms * (unsigned long)40000;
    
    if(ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        Nop();
        Nop();
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA0=!LATAbits.LATA0; // Se conmuta el LED D3 (RA0)
        //while(1);
    }
}

//Espera un determinado numero de microsegundos
void delay_us(unsigned int us){
    unsigned long ciclos = (unsigned long)us * (unsigned long)40;
    
    if (ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA2=!LATAbits.LATA2; // Se conmuta el LED D5 (RA2)
        //while(1);
    }
}