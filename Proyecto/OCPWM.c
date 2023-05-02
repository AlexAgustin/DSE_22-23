/*
Funciones relacionadas con la gestion de la senhal PWM

Contiene la funcion de inicializacion de las variables para la gestion de PWM y la funcion para visualizar duty0 en la pantalla.

Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"
#include "timers.h"

unsigned int DEF_DUTY_MIN=(PR20ms/20) * MINPWM;	// valor minimo y maximo por defecto de duty. Se calculan 
unsigned int DEF_DUTY_MAX=(PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int DUTY_MIN = DEF_DUTY_MIN;	// valor minimo y maximo de duty. Se calculan 
unsigned int DUTY_MAX = DEF_DUTY_MAX;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int OC_DUTY_MIN=(PR100us/0,1) * MINOCPWM;	// valor minimo y maximo por defecto de duty. Se calculan 
unsigned int OC_DUTY_MAX=(PR100us/0,1) * MAXOCPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int flag_DUTY = 1;  // duty0 se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = 1; //cuando duty0 cambia, se hace la conversion para visualizarlo en la pantalla  (flag a 1)

unsigned int duty0;
unsigned int duty1;
unsigned int duty2;
unsigned int duty3;
unsigned int duty4;
unsigned int estado_PWM;

unsigned int flag_num_duty=0;


void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC1R =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC1RS = OC1R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM sin prot
}

void inic_OC3 ()
{
    OC3CON=0; // Deshabilita modulo Output Compare
    
    //OC3CONbits.OCM=0b000;     // deshabilitar OC3
    OC3CONbits.OCTSEL=0;      // seleccionar T4 para el OC (OCTSEL= 0 -> Selecciona T4, OCTSEL= 1 -> T5)
    
    OC3R  =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC3RS = OC3R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC3CONbits.OCM=0b110;       // habilitar OC3 en modo PWM sin prot
}



void visualizar_Duty(){
    switch(flag_Duty_LCD)
    {
        case 1: //Caso: duty0
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty0);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 2: //Caso: duty1
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty1); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 3: //Caso: duty2
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty2); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 4: //Caso: duty3
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty3); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 5://Caso: duty4
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty4); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 6: //Caso: duty[0-4]
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty0);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty1); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty2); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty3); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty4); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC3RS); // Guardar valor de OC3RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 7: //Caso: min duty
            conversion_4dig(&Ventana_LCD[filadutymin][pos4dig], DUTY_MIN); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 8: //Caso: max duty
            conversion_4dig(&Ventana_LCD[filadutymax][pos4dig], DUTY_MAX); // Guardar valor del duty maximo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 9: //Caso: ruedas
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC3RS); // Guardar valor de OC3RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        default:
            //Inalcanzable
            break;
    }
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}


void inic_PWM(){
    estado_PWM=PWM0_ACTIVE; //Definir estado inicial
    duty0 = (DUTY_MAX+DUTY_MIN)/2; // Inicializar pulso con duracion intermedia (1,3ms))
    duty1 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty2 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty3 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty4 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    TRISEbits.TRISE10 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISEbits.TRISE11 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISEbits.TRISE12 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISEbits.TRISE13 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISEbits.TRISE14 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
}

void posicion_segura(){
    mover_servo(0,SECURE_DUTY);
    mover_servo(1,SECURE_DUTY);
    mover_servo(2,SECURE_DUTY);
    mover_servo(3,SECURE_DUTY);
    mover_servo(4,SECURE_DUTY);
}

/**
 * Mueve el servo correspondiente al duty num_duty a la posicion objetivo
 * @param num_duty Numerador asignado al duty que se quiere variar
 * @param objetivo Posicion a la que se quiere llegar
 */
void mover_servo(unsigned int num_duty, unsigned int objetivo){
    switch (num_duty){
        case 0:
            if (duty0<objetivo) while (duty0 >= objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty0 += 10;
            }
            else while (duty0 <= objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty0 -= 10;
            }
            break;
        case 1:
            if (duty1<objetivo) while (duty1 >= objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty1 += 10;
            }
            else while (duty1 <= objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty1 -= 10;
            }
            break;
        case 2:
            if (duty2<objetivo) while (duty2 >= objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty2 += 10;
            }
            else while (duty2 <= objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty2 -= 10;
            }
            break;
        case 3:
            if (duty3<objetivo) while (duty3 >= objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty3 += 10;
            }
            else while (duty3 <= objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty3 -= 10;
            }
            break;
        case 4:
            if (duty4<objetivo) while (duty4 >= objetivo){  //si duty tiene que incrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty4 += 10;
            }
            else while (duty4 <= objetivo){                 // si duty esta en el valor objetivo o tiene que decrementar, esperar a que el temporizador haga que incremente
                Delay_ms(10);
                duty4 -= 10;
            }
            break;
        default:
            break;
    }
}