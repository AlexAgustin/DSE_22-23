/* Funciones para trabajar con el modulo UART2
================================================
Comentarios!!!!!!!!!!!
Autorxs: 
Fecha:
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "timers.h"

#define UART_HOME 0
#define UART_DATA1 1
#define UART_NEXTLINE 2
#define UART_DATA2 3



void inic_UART2 ()
{
	
     // Velocidad de transmision
     // Hay que hacer solo una de las dos asignaciones siguientes
	U2BRG = BAUD_RATEREG_2_BRGH1;
	//U2BRG = BAUD_RATEREG_2_BRGH0;

     // U2MODE: habilitar el modulo (UARTEN), 8 bits, paridad par (PDSEL),
     // 1 bit de stop (STSEL), BRGH ...
	U2MODE = 0;
	U2MODEbits.BRGH=1;

     // U2STA: modo de interrupcion en el envio (UTXISEL), habilitacion del
     // envio (UTXEN), modo de interrupcion en la recepcion (URXISEL)
	U2STA = 0;

     // inicializacion de los bits IE e IF relacionados (IP si se quiere modificar)
	IFS1bits.U2RXIF=0;
    IFS1bits.U2TXIF=0;
	IEC1bits.U2RXIE=1;
    IEC1bits.U2TXIE=0;

            //IPC7bits.U2RXIP=xx;
	//IPC7bits.U2TXIP=xx;

     // interrupciones debidas a errores + bug
	IEC4bits.U2EIE=0;
	U2STAbits.OERR=0;

     // Habilitar el modulo y la linea TX.
     // Siempre al final y en ese orden!!!

	U2MODEbits.UARTEN=1;    // habilitar UART2
	U2STAbits.UTXEN=1;      // habilitar transmision tras habilitar modulo

	Delay_us(T_1BIT_US); 	// Esperar tiempo de 1 bit 
}

void _ISR_NO_PSV _U2RXInterrupt()
{
    char a = U2RXREG;
    switch (a){
        case 'p':
        case 'P':
            T7CONbits.TON = 0;
            break;
        case 'c':
        case 'C':
            T7CONbits.TON = 1;
            break;
        case 'i':
        case 'I':
            inicializar_crono = 1;
            break;
        default:
            break;
    }
    
    Ventana_LCD[1][15] = a;
    IFS1bits.U2RXIF=0; // Volver a poner el flag de interrupcion a 0
}

void _ISR_NO_PSV _U2TXInterrupt()
{
    static unsigned int estado_UART = 0;
    static int i = 0;
    switch (estado_UART){
        case UART_HOME: //Posicionamiento en la primera linea
            U2TXREG = home[i];
            i++;
            if (i==3){
                i = 0;
                estado_UART = UART_DATA1; // Cambio de estado, suguiente: envio de datos de la primera linea
            }
            break;
        case UART_DATA1: // Envio de los datos de la primera linea
            U2TXREG = Ventana_LCD[0][i];
            i++; // se incrementa la posicion
            if (i == 16) {
                i=0; //Si es la ultima iteracion, se vuelve a la posicion 0 y...
                estado_UART = UART_NEXTLINE; // y cambiamos de estado, siguiente: posicionamiento en la linea 2
            }
            break;
        case UART_NEXTLINE: //Posicionamiento en la segunda linea
            if (i==0) {
                U2TXREG = CR;
                i++;
            }else{
                U2TXREG = LF;
                i = 0;
                estado_UART = UART_DATA2; // Cambio de estado, suguiente: envio de datos de la primera linea
            }
            break;
        case UART_DATA2: // Envio de los datos de la segunda linea
            U2TXREG = Ventana_LCD[1][i];
            i++; // se incrementa la posicion
            if (i == 16) {
                i=0; //Si es la ultima iteracion, se vuelve a la posicion 0 y...
                estado_UART = UART_HOME; // y cambiamos de estado, siguiente: posicionamiento en la linea 1
            }
            break;
    }
}