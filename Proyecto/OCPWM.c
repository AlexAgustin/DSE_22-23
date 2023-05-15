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
#define SIT 1
#define PLAS 2
#define SHAKEIT_L 3
#define SHAKEIT_R 4
#define ENDRUTINA 5

unsigned int estado_PWM;

unsigned int OC_DUTY_MIN=(PR100us/0.1) * VEL_BAJA;	// valor minimo y maximo por defecto de duty. Se calculan 
unsigned int OC_DUTY_MAX=(PR100us/0.1) * VEL_ALTA;	// mediante los "define" PR20ms, MINPWM y MAXPWM

unsigned int flag_DUTY = 1;  // duty se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD; //cuando duty cambia, se hace la conversion para visualizarlo en la pantalla  (flag a 1)
unsigned int flag_calib = 1;
unsigned int flag_posicion_segura=0; //se lleva el brazo a una posicion segura para apagar si el flag esta a 1 (desde el programa principal)
unsigned int flag_num_duty = 0;
unsigned int flag_rutina_perro = 0;
unsigned int flag_inic_pwm=0;


unsigned int duty[5];
unsigned int objetivopwm[7];
unsigned int duty_min[5];
unsigned int duty_max[5];
unsigned int duty_seguro[5]={SECURE_DUTY_0,SECURE_DUTY_1,SECURE_DUTY_2,SECURE_DUTY_3,SECURE_DUTY_4};

void mover_servo(unsigned int num_duty, unsigned int objetivo);
void posicion_segura();
void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC1R =  0; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC1RS = OC1R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM sin prot
}

void inic_OC2 ()
{
    OC2CON=0; // Deshabilita modulo Output Compare
    
    //OC2CONbits.OCM=0b000;     // deshabilitar OC2
    OC2CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC2R  =  0; 		// Inicializar pulso con duracion intermedia
    OC2RS = OC2R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC2CONbits.OCM=0b110;       // habilitar OC2 en modo PWM sin prot
}


void inic_OC3 ()
{
    OC3CON=0; // Deshabilita modulo Output Compare
    
    //OC3CONbits.OCM=0b000;     // deshabilitar OC3 
    OC3CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC3R = 0; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC3RS = OC3R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC3CONbits.OCM=0b110;       // habilitar OC3 en modo PWM sin prot
}

void inic_OC4 ()
{
    OC4CON=0; // Deshabilita modulo Output Compare
    
    //OC4CONbits.OCM=0b000;     // deshabilitar OC4
    OC4CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC4R  =  0; 		// Inicializar pulso con duracion intermedia (0.0065ms)) (duty cycle para el primer pulso PWM)
    OC4RS = OC4R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC4CONbits.OCM=0b110;       // habilitar OC4 en modo PWM sin prot
}

void inic_Ruedas(){
    TRISBbits.TRISB8=0;  
    TRISBbits.TRISB9=0;
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
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmin], duty_min[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmin], duty_min[DUTY1]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmin], duty_min[DUTY2]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmin], duty_min[DUTY3]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmin], duty_min[DUTY4]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYMAX: //Caso: max duty
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmax], duty_max[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmax], duty_max[DUTY1]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmax], duty_max[DUTY2]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmax], duty_max[DUTY3]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmax], duty_max[DUTY4]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERCALIB:
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmin], duty_min[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmin], duty_min[DUTY1]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmin], duty_min[DUTY2]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmin], duty_min[DUTY3]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmin], duty_min[DUTY4]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmax], duty_max[DUTY0]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmax], duty_max[DUTY1]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmax], duty_max[DUTY2]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmax], duty_max[DUTY3]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmax], duty_max[DUTY4]); // Guardar valor del duty minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYOC: //Caso: ruedas
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyl],OC1RS); // Guardar valor de OC1RS en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaruedas][posdutyr],OC3RS); // Guardar valor de OC3RS en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYALL: //Caso: duty[0-4]
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
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
    flag_inic_pwm=1;
    
    //int duracion_intermedia = (DEF_DUTY_MAX + DEF_DUTY_MIN) / 2;
    estado_PWM = PWM0_ACTIVE; //Definir estado inicial
    
    duty[DUTY0]=(DEF_DUTY_MAX + DEF_DUTY_MIN) / 2;
    duty[DUTY1]=duty_max[DUTY1];
    duty[DUTY2]=duty_max[DUTY2];
    
    posicion_segura();

    TRISDbits.TRISD8 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISDbits.TRISD9 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISDbits.TRISD10 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISDbits.TRISD11 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISDbits.TRISD12 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
    flag_Duty_LCD = VERDUTYALL;
}

void inic_calib(){
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
    
    flag_Duty_LCD = VERCALIB;
}


void posicion_segura(){
    if (duty[DUTY0] != duty_seguro[DUTY0]) {
        objetivopwm[DUTY0] = duty_seguro[DUTY0];
        Nop();
        Nop();
        reached--;
    }
    if (duty[DUTY1] != duty_seguro[DUTY1]) {
        objetivopwm[DUTY1] = duty_seguro[DUTY1];
        Nop();
        Nop();
        reached--;
    }
    if (duty[DUTY2] != duty_seguro[DUTY2]) {
        objetivopwm[DUTY2] = duty_seguro[DUTY2];
        Nop();
        Nop();
        reached--;
    }
    if (duty[DUTY3] != duty_seguro[DUTY3]) {
        objetivopwm[DUTY3] = duty_seguro[DUTY3];
        Nop();
        Nop();
        reached--;
    }
    if (duty[DUTY4] != duty_seguro[DUTY4]) {
        objetivopwm[DUTY4] = duty_seguro[DUTY4];
        Nop();
        Nop();
        reached--;
    }
    flag_posicion_segura = 0;
    if (reached != 5) restart_Timer4_movservos(); 
}

void rutina_perro () {
    Nop();
    Nop();
    static unsigned int estado_rutina_perro=SIT;
    static unsigned int shakes=0;
    switch (estado_rutina_perro){
        case SIT:
            if (duty[DUTY0] != 951){
                objetivopwm[DUTY0]=951;
                reached--;
            }
            if (duty[DUTY1] != 1312){
                objetivopwm[DUTY1]=1312;
                reached--;
            }
            if (duty[DUTY2] != 1192){
                objetivopwm[DUTY2]=1192;
                reached--;
            }
            if (duty[DUTY3] != 1096){
                objetivopwm[DUTY3]=1096;
                reached--;
            }
            if (duty[DUTY4] != 1307){
                objetivopwm[DUTY4]=1307;
                reached--;
            }
            
            if (reached != 5) restart_Timer4_movservos();
            
            estado_rutina_perro=PLAS;
            break;
        case PLAS:
            if (duty[DUTY1] != 522){
                objetivopwm[DUTY1]=522;
                reached--;
            }
            if (duty[DUTY2] != 672){
                objetivopwm[DUTY2]=672;
                reached--;
            }
            if (duty[DUTY3] != 866){
                objetivopwm[DUTY3]=866;
                reached--;
            }
            
            if (reached != 5) restart_Timer4_movservos();
            
            estado_rutina_perro=SHAKEIT_L;
            break;
        case SHAKEIT_L:
            LATBbits.LATB8=1;
            LATBbits.LATB9=1;
        
            OC1RS = PR2 * VEL_ALTA;
            OC2RS = 0;
            
            if (duty[DUTY0] != 1051){
                objetivopwm[DUTY0]=1051;
                reached--;
            }
            if (duty[DUTY1] != 1307){
                objetivopwm[DUTY1]=1307;
                reached--;
            }
            if (duty[DUTY2] != 1307){
                objetivopwm[DUTY2]=1307;
                reached--;
            }
            if (duty[DUTY3] != 756){
                objetivopwm[DUTY3]=756;
                reached--;
            }
            if (duty[DUTY4] != 600){
                objetivopwm[DUTY4]=600;
                reached--;
            }
            if (reached != 5) restart_Timer4_movservos();
            if (shakes==5){
                estado_rutina_perro=ENDRUTINA;
                shakes=0;
            }else {
                estado_rutina_perro=SHAKEIT_R;
                shakes++;
            }
            break;
        case SHAKEIT_R:
            LATBbits.LATB8=1;
            LATBbits.LATB9=1;
        
            OC1RS = 0;
            OC2RS = PR2 * VEL_ALTA;
            if (duty[DUTY0] != 851){
                objetivopwm[DUTY0]=851;
                reached--;
            }
            if (reached != 5) restart_Timer4_movservos();
            estado_rutina_perro=SHAKEIT_L;
            break;
        default:
            flag_posicion_segura=1;
            flag_rutina_perro=0;
            estado_rutina_perro=SIT;
            LATBbits.LATB8=0;
            LATBbits.LATB9=0;
        
            OC1RS = 0;
            OC2RS = 0;
            break;
    }
    
}