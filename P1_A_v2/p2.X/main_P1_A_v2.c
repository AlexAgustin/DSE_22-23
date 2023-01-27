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
    
    TRISA = 0xFF80;
    
    TRISDbits.TRISD6 = 1;	// S3
    TRISDbits.TRISD7 = 1;	// S6
    TRISDbits.TRISD13 = 1;	// S4
    
    LATAbits.LATA1 = 1;
    LATAbits.LATA3 = 1;
    LATAbits.LATA5 = 1;
    LATAbits.LATA7 = 1;
    while (1) {
        
        while (!PORTDbits.RD13)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA0 = 1;
        }
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 1;
        
        while (!PORTAbits.RA7)
        {
            LATAbits.LATA3 = 0;
            LATAbits.LATA2 = 1;
        }
        LATAbits.LATA2 = 0;
        LATAbits.LATA3 = 1;
        
        while (!PORTDbits.RD7)
        {
            LATAbits.LATA5 = 0;
            LATAbits.LATA4 = 1;
        }
        LATAbits.LATA4 = 0;
        LATAbits.LATA5 = 1;
        
        while (!PORTDbits.RD6)
        {
            LATAbits.LATA7 = 0;
            LATAbits.LATA6 = 1;
        }
        LATAbits.LATA6 = 0;
        LATAbits.LATA7 = 1;
    }
    return (EXIT_SUCCESS);
}

