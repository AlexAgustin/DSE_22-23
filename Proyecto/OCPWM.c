/*
Funciones relacionadas con la gestion de la senhal PWM

Contiene la funcion de inicializacion de las variables para la gestion de PWM y la funcion para visualizar duty en la pantalla.

Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"
#include "timers.h"

unsigned int DUTY_MIN = DEF_DUTY_MIN;	// valor minimo y maximo de duty. Se calculan 
unsigned int DUTY_MAX = DEF_DUTY_MAX;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int OC_DUTY_MIN=(PR100us/0.1) * MINOCPWM;	// valor minimo y maximo por defecto de duty. Se calculan 
unsigned int OC_DUTY_MAX=(PR100us/0.1) * MAXOCPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int flag_DUTY = 1;  // duty se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = 9; //cuando duty cambia, se hace la conversion para visualizarlo en la pantalla  (flag a 1)
unsigned int flag_calib = 1;
unsigned int flag_posicion_segura=0; //se lleva el brazo a una posicion segura para apagar si el flag esta a 1 (desde el programa principal)

unsigned int duty[5];
unsigned int estado_PWM;

unsigned int flag_num_duty=0;

void mover_servo(unsigned int num_duty, unsigned int objetivo);
void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC1R =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC1RS = OC1R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM sin prot
}

void inic_OC2 ()
{
    OC2CON=0; // Deshabilita modulo Output Compare
    
    //OC2CONbits.OCM=0b000;     // deshabilitar OC2
    OC2CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC2R  =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC2RS = OC2R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC2CONbits.OCM=0b110;       // habilitar OC2 en modo PWM sin prot
}



void visualizar_Duty(){
    switch(flag_Duty_LCD)
    {
        case 1: //Caso: duty0
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 2: //Caso: duty1
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 3: //Caso: duty2
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 4: //Caso: duty3
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 5://Caso: duty4
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 6: //Caso: min duty
            conversion_4dig(&Ventana_LCD[filadutymin][pos4dig], DUTY_MIN); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 7: //Caso: max duty
            conversion_4dig(&Ventana_LCD[filadutymax][pos4dig], DUTY_MAX); // Guardar valor del duty maximo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 8: //Caso: ruedas
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC2RS); // Guardar valor de OC2RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 9: //Caso: duty[0-4]
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filadutymin][pos4dig], DUTY_MIN); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filadutymax][pos4dig], DUTY_MAX); // Guardar valor del duty maximo en Ventana_LCD para su visualizacion en la pantalla
            //conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            //conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC2RS); // Guardar valor de OC2RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        default:
            //Inalcanzable
            break;
    }
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}


void inic_PWM(){
    int duracion_intermedia = (DUTY_MAX + DUTY_MIN) / 2;
    estado_PWM = PWM0_ACTIVE; //Definir estado inicial
    mover_servo(DUTY0, duracion_intermedia); // Inicializar pulso con duracion intermedia (1,3ms))
    mover_servo(DUTY1, duracion_intermedia); // Inicializar pulso con duracion intermedia (1,3ms))
    mover_servo(DUTY2, duracion_intermedia); // Inicializar pulso con duracion intermedia (1,3ms))
    mover_servo(DUTY3, duracion_intermedia); // Inicializar pulso con duracion intermedia (1,3ms))
    mover_servo(DUTY4, duracion_intermedia); // Inicializar pulso con duracion intermedia (1,3ms))

    TRISDbits.TRISD8 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISDbits.TRISD9 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISDbits.TRISD10 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISDbits.TRISD11 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISDbits.TRISD12 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
}

void posicion_segura(){
    mover_servo(DUTY0,SECURE_DUTY_0);
    mover_servo(DUTY1,SECURE_DUTY_1);
    mover_servo(DUTY2,SECURE_DUTY_2);
    mover_servo(DUTY3,SECURE_DUTY_3);
    mover_servo(DUTY4,SECURE_DUTY_4);
    flag_posicion_segura = 0;
}

/**
 * Mueve el servo correspondiente al duty num_duty a la posicion objetivo
 * @param num_duty Numerador asignado al duty que se quiere variar
 * @param objetivo Posicion a la que se quiere llegar
 */
void mover_servo(unsigned int num_duty, unsigned int objetivo){
    switch (num_duty){
        case DUTY0:
            if (duty[DUTY0]<objetivo) while (duty[DUTY0] < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY0] += 5;
            }
            else while (duty[DUTY0] > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY0] -= 5;
            }
            duty[DUTY0] = objetivo;
            break;
        case DUTY1:
            if (duty[DUTY1]<objetivo) while (duty[DUTY1] < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY1] += 5;
            }
            else while (duty[DUTY1] > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY1] -= 5;
            }
            break;
            duty[DUTY1] = objetivo;
        case DUTY2:
            if (duty[DUTY2]<objetivo) while (duty[DUTY2] < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY2] += 5;
            }
            else while (duty[DUTY2] > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY2] -= 5;
            }
            break;
            duty[DUTY2] = objetivo;
        case DUTY3:
            if (duty[DUTY3]<objetivo) while (duty[DUTY3] < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY3] += 5;
            }
            else while (duty[DUTY3] > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY3] -= 5;
            }
            break;
            duty[DUTY3] = objetivo;
        case DUTY4:
            if (duty[DUTY4]<objetivo) while (duty[DUTY4] < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY4] += 5;
            }
            else while (duty[DUTY4] > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty[DUTY4] -= 5;
            }
            duty[DUTY4] = objetivo;
            break;
        case DUTYOC1:
            if (OC1RS<objetivo) while (OC1RS < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                OC1RS += 5;
            }
            else while (OC1RS > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                OC1RS -= 5;
            }
            OC1RS = objetivo;

            break;
        case DUTYOC2:
            if (OC2RS<objetivo) while (OC2RS < objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                OC2RS += 5;
            }
            else while (OC2RS > objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                OC2RS -= 5;
            }
            break;
        default:
            break;
    }
}