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

//Estados
#define LAPIZ 0
#define STAR 1
#define CASA 1


unsigned int estado_PWM;

unsigned int OC_DUTY_MIN=(PR100us/0.1) * MINOCPWM;	// valor minimo y maximo por defecto de duty. Se calculan 
unsigned int OC_DUTY_MAX=(PR100us/0.1) * MAXOCPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int flag_DUTY = 1;  // duty se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = VERDUTYALL; //cuando duty cambia, se hace la conversion para visualizarlo en la pantalla  (flag a 1)
unsigned int flag_calib = 1;
unsigned int flag_posicion_segura=0; //se lleva el brazo a una posicion segura para apagar si el flag esta a 1 (desde el programa principal)
unsigned int flag_num_duty = 0;
unsigned int flag_estrella = 0;
unsigned int flag_casa = 0;
unsigned int flag_inic_pwm=0;


unsigned int duty[5];
unsigned int objetivopwm[7];
unsigned int duty_min[5];
unsigned int duty_max[5];

void mover_servo(unsigned int num_duty, unsigned int objetivo);
void posicion_segura();
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


void inic_OC3 ()
{
    OC3CON=0; // Deshabilita modulo Output Compare
    
    //OC3CONbits.OCM=0b000;     // deshabilitar OC3 
    OC3CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC3R =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC3RS = OC3R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC3CONbits.OCM=0b110;       // habilitar OC3 en modo PWM sin prot
}

void inic_OC4 ()
{
    OC4CON=0; // Deshabilita modulo Output Compare
    
    //OC4CONbits.OCM=0b000;     // deshabilitar OC4
    OC4CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC4R  =  (OC_DUTY_MAX + OC_DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC4RS = OC4R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC4CONbits.OCM=0b110;       // habilitar OC4 en modo PWM sin prot
}

void visualizar_Duty(){
    switch(flag_Duty_LCD)
    {
        case VERDUTY0: //Caso: duty0
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTY1: //Caso: duty1
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTY2: //Caso: duty2
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTY3: //Caso: duty3
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTY4://Caso: duty4
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYMIN: //Caso: min duty
            conversion_4dig(&Ventana_LCD[filadutymin][pos4dig], duty_min[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYMAX: //Caso: max duty
            conversion_4dig(&Ventana_LCD[filadutymax][pos4dig], duty_min[DUTY0]); // Guardar valor del duty maximo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case 8: //Caso: ruedas
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC2RS); // Guardar valor de OC2RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYALL: //Caso: duty[0-4]
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filadutymin][pos4dig], duty_min[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filadutymax][pos4dig], duty_max[DUTY0]); // Guardar valor del duty maximo en Ventana_LCD para su visualizacion en la pantalla
            //conversion_4dig(&Ventana_LCD[filaruedas][posdutyl], OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            //conversion_4dig(&Ventana_LCD[filaruedas][posdutyr], OC2RS); // Guardar valor de OC2RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYADC: //Caso: duty0 duty2 duty4
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            break;
        default:
            //Inalcanzable
            break;
    }
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}


void inic_PWM(){
    T4CONbits.TON = 1;	// encender el timer
    
    flag_inic_pwm=1;
    
    //int duracion_intermedia = (DEF_DUTY_MAX + DEF_DUTY_MIN) / 2;
    estado_PWM = PWM0_ACTIVE; //Definir estado inicial
    
    duty[DUTY1]=DEF_DUTY_MAX;
    duty[DUTY2]=DEF_DUTY_MAX;
    
    /*objetivopwm[DUTY0] = 951; // Inicializar pulso con duracion intermedia (1,3ms))
    objetivopwm[DUTY1] = duracion_intermedia; // Inicializar pulso con duracion intermedia (1,3ms))
    objetivopwm[DUTY2] = duracion_intermedia; // Inicializar pulso con duracion intermedia (1,3ms))
    objetivopwm[DUTY3] = duracion_intermedia; // Inicializar pulso con duracion intermedia (1,3ms))
    objetivopwm[DUTY4] = duracion_intermedia; // Inicializar pulso con duracion intermedia (1,3ms))*/

    posicion_segura();
    
    //Inicializar duty minimo: valor por defecto
    duty_min[DUTY0] = DEF_DUTY_MIN;
    duty_min[DUTY1] = DEF_DUTY_MIN;
    duty_min[DUTY2] = DEF_DUTY_MIN;
    duty_min[DUTY3] = DEF_DUTY_MIN;
    duty_min[DUTY4] = DEF_DUTY_MIN;

    //Inicializar duty maximo: valor por defecto
    duty_max[DUTY0] = DEF_DUTY_MAX;
    duty_max[DUTY1] = DEF_DUTY_MAX;
    duty_max[DUTY2] = DEF_DUTY_MAX;
    duty_max[DUTY3] = DEF_DUTY_MAX;
    duty_max[DUTY4] = DEF_DUTY_MAX;

    TRISDbits.TRISD8 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISDbits.TRISD9 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISDbits.TRISD10 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISDbits.TRISD11 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISDbits.TRISD12 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
}

void posicion_segura(){
    T4CONbits.TON = 1;	// encender el timer
    objetivopwm[DUTY0] = SECURE_DUTY_0;
    objetivopwm[DUTY1] = SECURE_DUTY_1;
    objetivopwm[DUTY2] = SECURE_DUTY_2;
    objetivopwm[DUTY3] = SECURE_DUTY_3;
    objetivopwm[DUTY4] = SECURE_DUTY_4;
    flag_posicion_segura = 0;
}



void dibujar_estrella(){
    /*
    static int linea = LAPIZ;
    static int index = 0;
    static int pos_star0[8] = {POS_STAR0_0, POS_STAR1_0, POS_STAR2_0, POS_STAR3_0, POS_STAR4_0};
    static int pos_star1[8] = {POS_STAR0_1, POS_STAR1_1, POS_STAR2_1, POS_STAR3_1, POS_STAR4_1};
    static int pos_star2[8] = {POS_STAR0_2, POS_STAR1_2, POS_STAR2_2, POS_STAR3_2, POS_STAR4_2};
    static int pos_star3[8] = {POS_STAR0_3, POS_STAR1_3, POS_STAR2_3, POS_STAR3_3, POS_STAR4_3};


    switch(linea)
    {
        case LAPIZ:
            objetivopwm[DUTY4] = POS_LAPIZ_4;
            linea = STAR1;
        break;

        case STAR:
            objetivopwm[DUTY0] = pos_star0[index];
            objetivopwm[DUTY1] = pos_star1[index];
            objetivopwm[DUTY2] = pos_star2[index];
            objetivopwm[DUTY3] = pos_star3[index];
            index ++;

            if(index == 5){
                linea = LAPIZ;
                flag_star = 0;
                flag_posicion_segura = 1;
            }
        break;

        default:
            //Inalcanzable
        break;
    }

    flag_allreached = 0;
    */
}



void dibujar_casa(){
    /*
    static int linea = LAPIZ;
    static int index = 0;
    static int pos_casa0[8] = {POS_CASA0_0, POS_CASA1_0, POS_CASA2_0, POS_CASA3_0, POS_CASA4_0, POS_CASA5_0, POS_CASA6_0, POS_CASA7_0};
    static int pos_casa1[8] = {POS_CASA0_1, POS_CASA1_1, POS_CASA2_1, POS_CASA3_1, POS_CASA4_1, POS_CASA5_1, POS_CASA6_1, POS_CASA7_1};
    static int pos_casa2[8] = {POS_CASA0_2, POS_CASA1_2, POS_CASA2_2, POS_CASA3_2, POS_CASA4_2, POS_CASA5_2, POS_CASA6_2, POS_CASA7_2};
    static int pos_casa3[8] = {POS_CASA0_3, POS_CASA1_3, POS_CASA2_3, POS_CASA3_3, POS_CASA4_3, POS_CASA5_3, POS_CASA6_3, POS_CASA7_3};

    switch(linea)
    {
        case LAPIZ:
            objetivopwm[DUTY4] = POS_LAPIZ_4;
            linea = CASA1;
        break;

        case CASA:
            objetivopwm[DUTY0] = pos_casa0[index];
            objetivopwm[DUTY1] = pos_casa1[index];
            objetivopwm[DUTY2] = pos_casa2[index];
            objetivopwm[DUTY3] = pos_casa3[index];
            index ++;

            if(index == 8){
                linea = LAPIZ;
                flag_casa = 0;
                flag_posicion_segura = 1;
            }
        break;

        default:
            //Inalcanzable
        break;
    }

    flag_allreached = 0;
    */
}

