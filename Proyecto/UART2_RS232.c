/* Funciones para trabajar con el modulo UART2
================================================
 * Contiene las funciones de inicializacion y rutinas de atencion
 * a las interrupciones del emisor y receptor del modulo UART2.
Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "timers.h"
#include "OCPWM.h"

#define UART_HOME 0
#define UART_DATA 1

void inic_UART2 ()
{
     // Velocidad de transmision
     // Hay que hacer solo una de las dos asignaciones siguientes
	U2BRG = BAUD_RATEREG_2_BRGH1;  // Utilizamos el BRGH para velocidades altas
	//U2BRG = BAUD_RATEREG_2_BRGH0;

     // U2MODE: habilitar el modulo (UARTEN), 8 bits, no paridad,
     // 1 bit de stop (STSEL), BRGH ...
	U2MODE = 0; //Deshabilitado
	U2MODEbits.BRGH=1; //Uso de velocidades altas

     // U2STA: modo de interrupcion en el envio (UTXISEL), habilitacion del
     // envio (UTXEN), modo de interrupcion en la recepcion (URXISEL)
	U2STA = 0; //Deshabilitado

     // inicializacion de los bits IE e IF relacionados (IP si se quiere modificar)
	IFS1bits.U2RXIF=0; // puesta a 0 del flag IF del receptor
    IFS1bits.U2TXIF=0; // puesta a 0 del flag IF del emisor
	IEC1bits.U2RXIE=1; // habilitacion de las interrupciones del receptor
    IEC1bits.U2TXIE=1; // habilitacion de las interrupciones del emisor

    //IPC7bits.U2RXIP=xx;
	//IPC7bits.U2TXIP=xx;

     // interrupciones debidas a errores + bug
	IEC4bits.U2EIE=0; //deshabilitado
	U2STAbits.OERR=0; //deshabilitado

     // Habilitar el modulo y la linea TX.
     // Siempre al final y en ese orden!!!

	U2MODEbits.UARTEN=1;    // habilitar UART2
	U2STAbits.UTXEN=1;      // habilitar transmision tras habilitar modulo

	Delay_us(T_1BIT_US); 	// Esperar tiempo de 1 bit 
}

void _ISR_NO_PSV _U2RXInterrupt()
{
    char caracter = U2RXREG;
    switch (caracter){ // switch de comprobacion del caracter recibido
        case 'p':
        case 'P': //si es P o p, se para el temporizador
            T7CONbits.TON = 0;
            break;
        case 'c':
        case 'C': //si es C o c, se pone en marcha el temporizador
            T7CONbits.TON = 1;
            break;
        case 'i':
        case 'I': // si es I o i, se inicializa el temporizador a 0
            inicializar_crono = 1; //Poner el flag a uno para no alargar la rutina de atencion
            break;
        case 'r': //si es r, se mueve el servo incrementando el valor de duty0 (+10) si se respetan los limites (<= DUTY_MAX) y si flag_DUTY == 1 (se gestiona duty0 por UART)
            if(flag_DUTY && duty0+10<=DUTY_MAX) { // antes de mover el servo, se comprueba si al moverlo se seguiria dentro de los limites y si flag_DUTY == 1 (se gestiona duty0 por UART)
               duty0+=10; //Incrementar el valor de duty0: +10
               flag_Duty_LCD = 1; // Poner a 1 el flag para guardar el nuevo valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;
        case 'R': //si es R, se mueve el servo incrementando el valor de duty1 (+10) si se respetan los limites (<= DUTY_MAX)
            if(duty1+10<=DUTY_MAX) { // antes de mover el servo, se comprueba si al moverlo se seguiria dentro de los limites
               duty1+=10; //Incrementar el valor de duty1: +10
               //flag_Duty_LCD = 2; // Poner a 2 el flag para guardar el nuevo valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;
        case 'l': //si es l, se mueve el servo decrementando el valor de duty0 (-10) si se respetan los limites (>= DUTY_MIN) y si flag_DUTY == 1 (se gestiona duty0 por UART)
            if(flag_DUTY && duty0-10>=DUTY_MIN) { // antes de mover el servo, se comprueba si al moverlo se seguiria dentro de los limites y si flag_DUTY == 1 (se gestiona duty0 por UART)
                duty0-=10; // Decrementar el valor de duty0: -10
                flag_Duty_LCD = 1; // Poner a 1 el flag para guardar el nuevo valor de duty0 en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 'L':  //si es L, se mueve el servo decrementando el valor de duty1 (-10) si se respetan los limites (>= DUTY_MIN)
            if(duty1-10>=DUTY_MIN) { // antes de mover el servo, se comprueba si al moverlo se seguiria dentro de los limites
                duty1-=10; // Decrementar el valor de duty1: -10
                //flag_Duty_LCD = 2; // Poner a 2 el flag para guardar el nuevo valor de duty1 en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        default:
            break;
    }
    
    Ventana_LCD[1][15] = caracter; // anadir en la ultima posicion de ventana el carater recibido para obtener feedback
    IFS1bits.U2RXIF=0; // Volver a poner el flag de interrupcion a 0
}

void _ISR_NO_PSV _U2TXInterrupt()
{
    static unsigned int estado_UART = UART_HOME;
    static int caracter = 0;
    static int fila = 0;
    switch (estado_UART){ // switch que funciona como automata del emisor de UART
        case UART_HOME: //Posicionamiento al principio de la pantalla
            U2TXREG = home[caracter]; //se envia el comando para ello en tres partes
            caracter++;
            if (caracter==3){
                caracter = 0; //Si es la ultima iteracion, se pone a cero
                estado_UART = UART_DATA; // Cambio de estado, suguiente: envio de datos de la primera linea
            }
            break;
        case UART_DATA: // Envio de los datos de la linea
            U2TXREG = Ventana_LCD[fila][caracter]; //se envia un caracter
            caracter++; // se incrementa la posicion
            
            if (caracter == 18) {
                caracter=0; //Si es la ultima iteracion, se vuelve a la posicion 0
                fila++; //Salto a la siguiente linea a mostrar
                if (fila==18) {
                    estado_UART = UART_HOME; // volvemos al principio
                    fila=0;
                }
            }
            break;
        default:
            break; //Inalcanzable
    }
    IFS1bits.U2TXIF=0; // Volver a poner el flag de interrupcion a 0
}