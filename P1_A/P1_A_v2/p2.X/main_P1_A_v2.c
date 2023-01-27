/* 
 * File:   main_P1_A_v2.c
 * Author: xelab
 *
 * Created on 26 de enero de 2023, 16:25
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24HJ256GP610A.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    TRISA = 0xFF00;
    
    TRISDbits.TRISD6 = 1;	// S3
    TRISDbits.TRISD7 = 1;	// S6
    TRISDbits.TRISD13 = 1;	// S4
    
    LATAbits.LATA1 = 1;
    LATAbits.LATA3 = 1;
    LATAbits.LATA5 = 1;
    LATAbits.LATA7 = 1;
    while (1) {
        
        if (!PORTDbits.RD13)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA0 = 1;
        }else{
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 1;
        }
        
        TRISAbits.TRISA7 = 1;
        Nop();
        Nop();
        Nop();
        if (!PORTAbits.RA7)
        {
            LATAbits.LATA3 = 0;
            LATAbits.LATA2 = 1;
        }else{
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;
        }
        TRISAbits.TRISA7 = 0;
        
        if (!PORTDbits.RD7)
        {
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 1;
        }else{
            LATAbits.LATA4 = 0;
            LATAbits.LATA5 = 1;
        }
        
        if (!PORTDbits.RD6)
        {
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 1;
        }else{
            LATAbits.LATA6 = 0;
            LATAbits.LATA7 = 1;
        }
    }
    return (EXIT_SUCCESS);
}

