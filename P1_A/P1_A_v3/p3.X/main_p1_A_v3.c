/* 
 * File:   main_p1_A_v3.c
 * Author: Alex y Amanda
 *
 * Created on 27 de enero de 2023, 14:53
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24HJ256GP610A.h"

/*
 * 
 */
int main(int argc, char** argv) {

    AD1PCFGH = 0xffff;
    
    TRISA = 0xFF00;
    
    LATAbits.LATA7 = 1;
    
    while (1) {
        TRISAbits.TRISA7 = 1;
        Nop();Nop();Nop();
        while (!PORTAbits.RA7)
        {
            TRISAbits.TRISA7 = 0;
            Nop();
            Nop();
            Nop();
            LATAbits.LATA7 = 0;
            LATAbits.LATA2 = 1;
        }
        TRISAbits.TRISA7 = 0;
        Nop();Nop();Nop();
        LATAbits.LATA2 = 0;
        LATAbits.LATA7 = 1;
    }
    return (EXIT_SUCCESS);
}

