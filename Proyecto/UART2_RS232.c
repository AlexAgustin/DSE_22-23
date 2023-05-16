/* Funciones para trabajar con el modulo UART2
================================================
 * Contiene las funciones de inicializacion y rutinas de atencion
 * a las interrupciones del emisor y receptor del modulo UART2.
Autores: Alex y Amanda
Fecha: Mayo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "timers.h"
#include "OCPWM.h"
#include "LCD.h"
#include "srf08.h"

#define UART_HOME 0
#define UART_DATA 1

unsigned int flag_exit = 0; //Flag para finalizar el programa (1)


// Funcion que inicializa el modulo UART2
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

	U2MODEbits.UARTEN=1;    // habilitar UART2
	U2STAbits.UTXEN=1;      // habilitar transmision tras habilitar modulo

	Delay_us(T_1BIT_US); 	// Esperar tiempo de 1 bit 
}

// Rutina de atencion a la interrupcion del receptor del modulo UART2
// Segun el caracter recibido, lleva a cabo las acciones pertinentes
void _ISR_NO_PSV _U2RXInterrupt()
{
    static int duty_cur = DUTY0;
    static unsigned int flag_busy = 0;
    char caracter = U2RXREG;
    static unsigned int ismin=0;

    if(reached != 5) flag_busy = 1; 
    else  flag_busy = 0;

    switch (caracter){ // switch de comprobacion del caracter recibido
        case 'p':
        case 'P': //si es P o p, se para el cronometro
            T7CONbits.TON = 0;
            break;
        case 'c':
        case 'C': //si es C o c, se pone en marcha el cronometro
            T7CONbits.TON = 1;
            break;
        case 'i':
        case 'I': // si es I o i, se inicializa el cronometro a 0
            inicializar_crono = 1; //Poner el flag para inicializar el cronometro a uno para no alargar la rutina de atencion
            break;
        case 'x':
        case 'X': // si es x o X, se alterna la gestion de las senhales PWM entre UART y las entradas analogicas
            flag_DUTY = !flag_DUTY; // Conmutar el flag que determina el modo de gestion de las senhales PWM entre UART (1) y entradas analogicas (0)
            break;
        case 'd':
        case 'D': //si es d o D, se pone a 1 el flag que hara que el robot realice una rutina canina
            flag_rutina_perro = 1;
            break;
        case 'f':
        case 'F': //si es f o F, se fija el valor que esta siendo calibrado actualmente y se pasa a gestionar el siguiente
            if(flag_calib) // Si se esta gestionando el calibrado...
            {
                if(!ismin) { //si se estaba calibrando el valor maximo de cualquier duty
                    duty_max[duty_cur] = duty[duty_cur]; // asignar el valor actual del duty correspondiente como el valor maximo
                    if (duty[duty_cur]!=duty_seguro[duty_cur]){ //Si el servo no esta en una posicion segura...
                        objetivopwm[duty_cur]=duty_seguro[duty_cur]; //actualizar posicion objetivo con un valor seguro
                        reached--; //Actualizar cantidad de servos que no se encuentran en su posicion objetivo
                        restart_Timer4_movservos(); // Encender timer para la gestion del movimiento de los servos
                    }
                    if(reached ==5) flag_Duty_LCD = VERCALIB;
                }else{
                    duty_min[duty_cur] = duty[duty_cur]; // asignar el valor actual del duty siendo gestionado como el minimo
                    if (duty[duty_cur]!=duty_seguro[duty_cur]){ //Si el servo no esta en una posicion segura...
                        objetivopwm[duty_cur]=duty_seguro[duty_cur];//actualizar posicion objetivo con un valor seguro
                        reached--; //Actualizar cantidad de servos que no se encuentran en su posicion objetivo
                        restart_Timer4_movservos(); // Encender timer para la gestion del movimiento de los servos
                    }
                    switch (duty_cur){ //Switch para actualizar el duty a gestionar
                        case DUTY0: //Caso duty0
                            duty_cur = DUTY1; // pasar a calibrar DUTY1
                            break;
                        case DUTY1://Caso duty1
                            duty_cur = DUTY2; // pasar a calibrar DUTY2
                            break;
                        case DUTY2://Caso duty2
                            duty_cur = DUTY3; // pasar a calibrar DUTY3
                            break;
                        case DUTY3://Caso duty3
                            duty_cur = DUTY4; // pasar a calibrar DUTY4
                            break;
                        case DUTY4://Caso duty4
                            flag_calib = 0; //Fin de la gestion del calibrado
                            break;

                        default:
                            break; //Inalcanzable
                    }

                    if(reached ==5) flag_Duty_LCD = VERCALIB;
                }
                
                ismin=!ismin; // Conmutar ismin: minimo (1) <=> maximo (0)
            }
            break;
            

        case 'R': 
            // si es R, se mueve el servo S1 incrementando el valor de duty0 (+5) si al moverlo se seguirian respetando los limites (<= duty_max[DUTY0]), 
            // si no se esta calibrando
            // si flag_DUTY == 1 (se gestiona duty0 por UART) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY0]+5<=duty_max[DUTY0]) { // Realizar comprobaciones
                duty[DUTY0]+=5; //Incrementar el valor de duty[DUTY0]: +5
                objetivopwm[DUTY0]= duty[DUTY0];
                flag_Duty_LCD = VERDUTY0; // Poner a VERDUTY0 el flag para guardar el nuevo valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;


        case 'L': 
            // si es L, se mueve el servo incrementando el valor de duty1 (+5) si al moverlo se seguirian respetando los limites (<= duty_max[DUTY1]), 
            // si no se esta calibrando
            // si no hay riesgo de choque (dis_media>=CHOQUE) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && duty[DUTY1]+5<=duty_max[DUTY1] && dis_media>=CHOQUE) { // Realizar comprobaciones
                duty[DUTY1]+=5; //Incrementar el valor de duty1: +5
                objetivopwm[DUTY1]= duty[DUTY1];
                flag_Duty_LCD = VERDUTY1; // Poner a VERDUTY1 el flag para guardar el nuevo valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;

        case 'W': 
            // si es W, se mueve el servo incrementando el valor de duty2 (+5) si al moverlo se seguirian respetando los limites (<= duty_max[DUTY2]) 
            // si no se esta calibrando
            // si flag_DUTY == 1 (se gestiona duty2 por UART), 
            // si no hay riesgo de choque (dis_media>=CHOQUE) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY2]+5<=duty_max[DUTY2] && dis_media>=CHOQUE) { // Realizar comprobaciones
                duty[DUTY2]+=5; //Incrementar el valor de duty2: +5
                objetivopwm[DUTY2]= duty[DUTY2];
                flag_Duty_LCD = VERDUTY2; // Poner a VERDUTY2 el flag para guardar el nuevo valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;

        case 'A': 
            // si es A, se mueve el servo incrementando el valor de duty[DUTY3] (+5) si al moverlo se seguirian respetando los limites (<= duty_max[DUTY3]) 
            // si no se esta calibrando
            // si no hay riesgo de choque (dis_media>=CHOQUE) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && duty[DUTY3]+5<=duty_max[DUTY3] && dis_media>=CHOQUE) { // Realizar comprobaciones
                duty[DUTY3]+=5; //Incrementar el valor de duty3: +5
                objetivopwm[DUTY3]= duty[DUTY3];
                flag_Duty_LCD = VERDUTY3; // Poner a VERDUTY3 el flag para guardar el nuevo valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;

        case 'S': 
            // si es S, se mueve el servo incrementando el valor de duty[DUTY4] (+5) si al moverlo se seguirian respetando los limites (<= duty_max[DUTY4]) 
            // si no se esta calibrando
            // si flag_DUTY == 1 (se gestiona duty[DUTY4] por UART) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY4]+5<=duty_max[DUTY4]) { // Realizar comprobaciones
                duty[DUTY4]+=5; //Incrementar el valor de duty4: +5
                objetivopwm[DUTY4]= duty[DUTY4];
                flag_Duty_LCD = VERDUTY4; // Poner a VERDUTY4 el flag para guardar el nuevo valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            }
            break;

        case 'M': 
            //si es M, se incrementa el valor de duty que se este calibrando
            //antes de actualizar el valor, se comprueba 
            // si al cambiarlo se seguiria dentro de los limites y si estamos en la fase de calibrado (flag_calib == 1)
            if(flag_calib && duty[duty_cur]+5 <= duty_max[duty_cur] && (!ismin || (ismin && duty[duty_cur]+5<=duty_seguro[duty_cur]))) { //Realizar comprobaciones
                duty[duty_cur]+=5; //Incrementar el valor del duty que se esta calibrando actualmente (duty_cur): +5
                flag_Duty_LCD = VERDUTYALL; // Poner a VERDUTYALL el flag para visualizar dutys
            }
            break;
        
        case 'r': 
            //si es r, se mueve el servo decrementando el valor de duty[DUTY0] (-5) 
            // si no se esta calibrando
            // si al moverlo se seguirian respetando los limites (>= duty_min) 
            // si flag_DUTY == 1 (se gestiona duty0 por UART) y 
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY0]-5>=duty_min[DUTY0]) { // realizar comprobaciones
                duty[DUTY0]-=5; // Decrementar el valor de duty0: -5
                objetivopwm[DUTY0]= duty[DUTY0];
                flag_Duty_LCD = VERDUTY0; // Poner a VERDUTY0 el flag para guardar el nuevo valor de duty0 en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 'l':  
            //si es l, se mueve el servo decrementando el valor de duty[DUTY1] (-5) 
            // si no se esta calibrando
            // si no hay riesgo de choque (dis_media>=CHOQUE)
            // si al moverlo se seguirian respetando los limites (>= duty_min) y
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && duty[DUTY1]-5>=duty_min[DUTY1] && dis_media>=CHOQUE) { // realizar comprobaciones
                duty[DUTY1]-=5; // Decrementar el valor de duty1: -5
                objetivopwm[DUTY1]= duty[DUTY1];
                flag_Duty_LCD = VERDUTY1; // Poner a 2 el flag para guardar el nuevo valor de duty[DUTY1] en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 'w': 
            //si es w, se mueve el servo decrementando el valor de duty[DUTY2] (-5) 
           // si no hay riesgo de choque (dis_media>=CHOQUE)
            // si no se esta calibrando
            // si al moverlo se seguirian respetando los limites (>= duty_min) 
            // si flag_DUTY == 1 (se gestiona duty2 por UART) y
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY2]-5>=duty_min[DUTY2] && dis_media>=CHOQUE) {  // realizar comprobaciones
                duty[DUTY2]-=5; // Decrementar el valor de duty[DUTY2]: -5
                objetivopwm[DUTY2]= duty[DUTY2];
                flag_Duty_LCD = VERDUTY2; // Poner a 3 el flag para guardar el nuevo valor de duty[DUTY2] en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 'a': 
            //si es a, se mueve el servo decrementando el valor de duty[DUTY3] (-5) 
            // si no se esta calibrando
            // si no hay riesgo de choque (dis_media>=CHOQUE)
            // si al moverlo se seguirian respetando los limites (>= duty_min) y
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && duty[DUTY3]-5>=duty_min[DUTY3] && dis_media>=CHOQUE) {  // realizar comprobaciones
                duty[DUTY3]-=5; // Decrementar el valor de duty[DUTY3]: -5
                objetivopwm[DUTY3]= duty[DUTY3];
                flag_Duty_LCD = VERDUTY3; // Poner a 4 el flag para guardar el nuevo valor de duty[DUTY3] en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 's': 
            //si es s, se mueve el servo decrementando el valor de duty[DUTY4] (-5) 
            // si no se esta calibrando
            // si al moverlo se seguirian respetando los limites (>= duty_min) 
            // si flag_DUTY == 1 (se gestiona duty4 por UART) y
            // si el servo no esta en movimiento actualmente (!flag_busy) ni se esta realizando la rutina canina
            if(!flag_calib && !flag_rutina_perro && !flag_busy && flag_DUTY && duty[DUTY4]-5>=duty_min[DUTY4]) {  // realizar comprobaciones
                duty[DUTY4]-=5; // Decrementar el valor de duty[DUTY4]: -5
                objetivopwm[DUTY4]= duty[DUTY4];
                flag_Duty_LCD = VERDUTY4; // Poner a 5 el flag para guardar el nuevo valor de duty[DUTY4] en Ventana_LCD para su su visualizacion en la pantalla
            }
            break;
        case 'm': 
            //si es m, se decrementa el valor de duty que se este calibrando
            // antes de actualizar el valor, se comprueba 
            // si al cambiarlo se seguiria dentro de los limites y si estamos en la fase de calibrado (flag_calib == 1)
            if(flag_calib && duty[duty_cur]-5>=duty_min[duty_cur] && (ismin || (!ismin && duty[duty_cur]-5>=duty_seguro[duty_cur]))) {
                duty[duty_cur]-=5; // Decrementar el valor de duty: -5
                flag_Duty_LCD = VERDUTYALL; // visualizar dutys
            }
            break;
        case '>': //si es >, se hace scroll hacia abajo de la informacion a mostrar en la LCD
            if(fila1<NFILAS-2) // Si no se estan mostrando en la LCD las dos ultimas lineas...
                {
                    // scroll hacia abajo de la informacion a mostrar en la LCD
                    fila1++; //Actualizar primera fila a mostrar (+1)
                    fila2++; //Actualizar segunda fila a mostrar (+1)
                }
            break;
        case '<': //si es <, se hace scroll hacia arriba de la informacion a mostrar en la LCD
            if(fila1>2) // Si no se estan mostrando en la LCD las dos primeras lineas...
                {
                    //scroll hacia arriba de la informacion a mostrar en la LCD
                    fila1--; //Actualizar primera fila a mostrar (-1)
                    fila2--; //Actualizar segunda fila a mostrar (-1)
                }
            break;
        case 'z':
        case 'Z': 
            // Si es 'Z' o 'z' se lleva el brazo a una posicion segura => flag_posicion_segura = 1
            flag_posicion_segura=1; 
            break;
        case 'q': 
        case 'Q': 
            // si es q o Q, se pone a 1 el flag que hara que el programa finalice
            flag_exit=1; 
            break;
        default:
            break;
    }
    
    Ventana_LCD[filacrono][poscarac] = caracter; // anhadir en la ultima posicion de la linea del cronometro en Ventana_LCD el carater recibido para obtener feedback
    IFS1bits.U2RXIF=0; // Volver a poner el flag de interrupcion a 0
}

// Rutina de atencion a la interrupcion del emisor del modulo UART2
// Envia los datos pertinentes a partir de un automata
void _ISR_NO_PSV _U2TXInterrupt()
{
    static unsigned int estado_UART = UART_HOME;
    static int caracter = 0;
    static int fila = 0;
    // switch que funciona como automata del emisor de UART
    switch (estado_UART){ //Segun el estado...
        case UART_HOME: //Posicionamiento al principio de la pantalla
            U2TXREG = home[caracter]; //se envia el comando para ello en tres partes
            caracter++; // Se incrementa el indice
            if (caracter==3){ //Si es el ultimo caracter de la operacion...
                caracter = 0; //Se pone el indice a cero
                estado_UART = UART_DATA; // Cambio de estado, siguiente: envio de los datos de las linea
            }
            break;
        case UART_DATA: // Envio de los datos de las lineas
            U2TXREG = Ventana_LCD[fila][caracter]; //se envia un caracter
            caracter++; // se incrementa la posicion
            
            if (caracter == NCOLUMNAS) { // Si se han enviado todos los datos de la linea...
                caracter=0; //Se vuelve a la posicion 0
                fila++; //Salto a la siguiente linea a mostrar
                if (fila==NFILAS) { //Si se han enviado todas las lineas...
                    estado_UART = UART_HOME; // volvemos al principio
                    fila = 0; // Se vuelve a la linea 0
                }
            }
            break;
        default:
            break; //Inalcanzable
    }
    IFS1bits.U2TXIF=0; // Volver a poner el flag de interrupcion a 0
}