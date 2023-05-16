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
//=======
#define SIT 1
#define PLAS 2
#define SHAKEIT_L 3
#define SHAKEIT_R 4
#define ENDRUTINA 5

unsigned int estado_PWM;

unsigned int flag_DUTY = 1;  // duty se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD; // cuando duty cambia, se hace la conversion para visualizarlo en la pantalla 
unsigned int flag_calib = 1; //permite realizar el calibrado
unsigned int flag_posicion_segura=0; //se lleva el brazo a una posicion segura para apagar si el flag esta a 1 (desde el programa principal)
unsigned int flag_num_duty = 0;
unsigned int flag_rutina_perro = 0; // el brazo realiza una rutina de entrenamiento canino
unsigned int flag_inic_pwm=0;


unsigned int duty[5];
unsigned int objetivopwm[7];
unsigned int duty_min[5];
unsigned int duty_max[5];
unsigned int duty_seguro[5]={SECURE_DUTY_0,SECURE_DUTY_1,SECURE_DUTY_2,SECURE_DUTY_3,SECURE_DUTY_4};

void mover_servo(unsigned int num_duty, unsigned int objetivo);
void posicion_segura();

// Inicializa el modulo OC1 para mover una rueda hacia adelante
void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC1R =  0; 		// Inicializar pulso a 0
    OC1RS = OC1R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM sin prot
}

// Inicializa el modulo OC2 para mover una rueda hacia adelante
void inic_OC2 ()
{
    OC2CON=0; // Deshabilita modulo Output Compare
    
    //OC2CONbits.OCM=0b000;     // deshabilitar OC2
    OC2CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC2R  =  0; 		// Inicializar pulso a 0
    OC2RS = OC2R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC2CONbits.OCM=0b110;       // habilitar OC2 en modo PWM sin prot
}

// Inicializa el modulo OC3 para mover una rueda hacia atras
void inic_OC3 ()
{
    OC3CON=0; // Deshabilita modulo Output Compare
    
    //OC3CONbits.OCM=0b000;     // deshabilitar OC3 
    OC3CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC3R = 0; 		// Inicializar pulso a 0
    OC3RS = OC3R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC3CONbits.OCM=0b110;       // habilitar OC3 en modo PWM sin prot
}

// Inicializa el modulo OC4 para mover una rueda hacia atras
void inic_OC4 ()
{
    OC4CON=0; // Deshabilita modulo Output Compare
    
    //OC4CONbits.OCM=0b000;     // deshabilitar OC4
    OC4CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2, OCTSEL= 1 -> T3)
    
    OC4R  =  0; 		// Inicializar pulso a 0
    OC4RS = OC4R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC4CONbits.OCM=0b110;       // habilitar OC4 en modo PWM sin prot
}

// Inicializa los pines requeridos para habilitar los motores de las ruedas (Enable)
void inic_Ruedas(){
    // Establecer pines (Enable) como pines de salida
    TRISBbits.TRISB8=0;   // Pin RB8
    TRISBbits.TRISB9=0;   // Pin RB9
}

//Se ocupa de actualizar Ventana_LCD para visualizar duty por pantalla
void visualizar_Duty(){
    switch(flag_Duty_LCD) // Se actualizan los valores pertinentes segun el estado
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
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla

            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmin], duty_min[DUTY0]); // Guardar valor del duty0 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmin], duty_min[DUTY1]); // Guardar valor del duty1 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmin], duty_min[DUTY2]); // Guardar valor del duty2 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmin], duty_min[DUTY3]); // Guardar valor del duty3 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmin], duty_min[DUTY4]); // Guardar valor del duty4 minimo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERDUTYMAX: //Caso: max duty
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla

            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmax], duty_max[DUTY0]); // Guardar valor del duty0 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmax], duty_max[DUTY1]); // Guardar valor del duty1 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmax], duty_max[DUTY2]); // Guardar valor del duty2 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmax], duty_max[DUTY3]); // Guardar valor del duty3 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmax], duty_max[DUTY4]); // Guardar valor del duty4 maximo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERSOLOCALIB:
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmin], duty_min[DUTY0]); // Guardar valor del duty0 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmin], duty_min[DUTY1]); // Guardar valor del duty1 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmin], duty_min[DUTY2]); // Guardar valor del duty2 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmin], duty_min[DUTY3]); // Guardar valor del duty3 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmin], duty_min[DUTY4]); // Guardar valor del duty4 minimo en Ventana_LCD para su visualizacion en la pantalla
            
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmax], duty_max[DUTY0]); // Guardar valor del duty0 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmax], duty_max[DUTY1]); // Guardar valor del duty1 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmax], duty_max[DUTY2]); // Guardar valor del duty2 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmax], duty_max[DUTY3]); // Guardar valor del duty3 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmax], duty_max[DUTY4]); // Guardar valor del duty4 maximo en Ventana_LCD para su visualizacion en la pantalla
            break;
        case VERCALIB: //Caso calibracion
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty[DUTY0]);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty[DUTY1]); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty[DUTY2]); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty[DUTY3]); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty[DUTY4]); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
            
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmin], duty_min[DUTY0]); // Guardar valor del duty0 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmin], duty_min[DUTY1]); // Guardar valor del duty1 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmin], duty_min[DUTY2]); // Guardar valor del duty2 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmin], duty_min[DUTY3]); // Guardar valor del duty3 minimo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmin], duty_min[DUTY4]); // Guardar valor del duty4 minimo en Ventana_LCD para su visualizacion en la pantalla
            
            conversion_4dig(&Ventana_LCD[filacalibs0][poscalibmax], duty_max[DUTY0]); // Guardar valor del duty0 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs1][poscalibmax], duty_max[DUTY1]); // Guardar valor del duty1 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs2][poscalibmax], duty_max[DUTY2]); // Guardar valor del duty2 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs3][poscalibmax], duty_max[DUTY3]); // Guardar valor del duty3 maximo en Ventana_LCD para su visualizacion en la pantalla
            conversion_4dig(&Ventana_LCD[filacalibs4][poscalibmax], duty_max[DUTY4]); // Guardar valor del duty4 maximo en Ventana_LCD para su visualizacion en la pantalla
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
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar los valores debidos de duty en Ventana_LCD para su visualizacion en la pantalla
}

// Inicializacion de las variables relacionadas con las senhales PWM
void inic_PWM(){
    flag_inic_pwm=1;
    
    estado_PWM = PWM0_ACTIVE; //Definir estado inicial
    
    // Inicializacion a partir de valores determinados para evitar movimientos bruscos iniciales
    duty[DUTY0]=(duty_max[DUTY0] + duty_min[DUTY0]) / 2; //Asignar valor intermedio a duty0
    duty[DUTY1]=duty_max[DUTY1]; //Asignar duty1 maximo a duty1
    duty[DUTY2]=duty_max[DUTY2]; //Asignar duty2 maximo a duty2
    
    posicion_segura(); // Llevar el brazo a una posicion segura

    TRISDbits.TRISD8 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISDbits.TRISD9 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISDbits.TRISD10 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISDbits.TRISD11 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISDbits.TRISD12 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
    flag_Duty_LCD = VERDUTYALL; // Visualizar todos los dutys
}

void inic_calib(){
    //Inicializar duty minimo: valor por defecto
    duty_min[DUTY0] = DEF_DUTY_MIN; //duty0
    duty_min[DUTY1] = DEF_DUTY_MIN; //duty1
    duty_min[DUTY2] = DEF_DUTY_MIN; //duty2
    duty_min[DUTY3] = DEF_DUTY_MIN; //duty3
    duty_min[DUTY4] = DEF_DUTY_MIN; //duty4
    
    //Inicializar duty maximo: valor por defecto
    duty_max[DUTY0] = DEF_DUTY_MAX; //duty0
    duty_max[DUTY1] = DEF_DUTY_MAX; //duty1
    duty_max[DUTY2] = DEF_DUTY_MAX; //duty2
    duty_max[DUTY3] = DEF_DUTY_MAX; //duty3
    duty_max[DUTY4] = DEF_DUTY_MAX; //duty4
    
    flag_Duty_LCD = VERSOLOCALIB; // Visualizar maximos y minimos
}

//Llevar el brazo a una posicion segura
void posicion_segura(){
    //Caso duty0
    if (duty[DUTY0] != duty_seguro[DUTY0]) {  //si no se encuentra en una posicion segura...
        objetivopwm[DUTY0] = duty_seguro[DUTY0]; //actualizar posicion objetivo
        reached--; // actualizar cantidad de servos en su posicion objetivo
    }
    //caso duty1
    if (duty[DUTY1] != duty_seguro[DUTY1]) { //si no se encuentra en una posicion segura...
        objetivopwm[DUTY1] = duty_seguro[DUTY1];//actualizar posicion objetivo
        reached--;// actualizar cantidad de servos en su posicion objetivo
    }
    //caso duty2
    if (duty[DUTY2] != duty_seguro[DUTY2]) { //si no se encuentra en una posicion segura...
        objetivopwm[DUTY2] = duty_seguro[DUTY2];//actualizar posicion objetivo
        reached--;// actualizar cantidad de servos en su posicion objetivo
    }
    //caso duty3
    if (duty[DUTY3] != duty_seguro[DUTY3]) { //si no se encuentra en una posicion segura...
        objetivopwm[DUTY3] = duty_seguro[DUTY3];//actualizar posicion objetivo
        reached--;// actualizar cantidad de servos en su posicion objetivo
    }
    //caso duty4
    if (duty[DUTY4] != duty_seguro[DUTY4]) {//si no se encuentra en una posicion segura...
        objetivopwm[DUTY4] = duty_seguro[DUTY4];//actualizar posicion objetivo
        reached--;// actualizar cantidad de servos en su posicion objetivo
    }
    flag_posicion_segura = 0; //Actualiar flag que da lugar a llevar el brazo a una posicion segura
    if (reached != 5) restart_Timer4_movservos();  // Si la cantidad de servos en su posicion objetivo no es 5 => gestionar movimiento de servos
}

// Funion que da lugar a la realizacion de una rutina canina
void rutina_perro () {
    static unsigned int estado_rutina_perro=SIT;
    static unsigned int shakes=0;
    switch (estado_rutina_perro){ //Segun el estado se llevan a cabo diferentes acciones
        case SIT: //Caso: sentado (SIT)
            //Se lleva cada servo a la posicion correspondiente
            if (duty[DUTY0] != 951){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY0]=951;//actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY1] != 1312){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY1]=1312; //actualizar posicion objetivo
                reached--;// actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY2] != 1192){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY2]=1192; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY3] != 1096){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY3]=1096; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY4] != 1307){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY4]=1307; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            
            if (reached != 5) restart_Timer4_movservos(); // Si la cantidad de servos en su posicion objetivo no es 5 => gestionar movimiento de servos
            
            estado_rutina_perro=PLAS; //Estado siguiente: tumbado (PLAS)
            break;
        case PLAS: //Caso tumbado (PLAS)
            if (duty[DUTY1] != 522){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY1]=522; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY2] != 672){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY2]=672; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY3] != 866){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY3]=866; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            
            if (reached != 5) restart_Timer4_movservos(); // Si la cantidad de servos en su posicion objetivo no es 5 => gestionar movimiento de servos
            
            estado_rutina_perro=SHAKEIT_L; // Estado siguiente: mover colita a la izda (SHAKEIT_L)
            break;

        case SHAKEIT_L: // Caso: mover colita a la izda (SHAKEIT_L)
            LATBbits.LATB8=1; //Enable = 1 (encender motor de rueda)
            LATBbits.LATB9=0; //Enable = 0 (apagar motor de rueda)
        
            OC1RS = PR2 * VEL_ALTA; // Velocida alta en la rueda
            OC2RS = 0; // Rueda quieta
            
            if (duty[DUTY0] != 1051){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY0]=1051; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY1] != 1307){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY1]=1307; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY2] != 1307){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY2]=1307; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY3] != 756){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY3]=756; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (duty[DUTY4] != 600){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY4]=600; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (reached != 5) restart_Timer4_movservos(); // Si la cantidad de servos en su posicion objetivo no es 5 => gestionar movimiento de servos
            if (shakes==5){ // Si se han dado 5 movimientos de colita... 
                estado_rutina_perro=ENDRUTINA; // Termina la rutina canina 
                shakes=0; // reseteamos movimientos de colita
            }else { //Menos de 5 movimientos de colita...
                estado_rutina_perro=SHAKEIT_R; // Estado siguiente: mover la colita a la derecha (SHAKEIT_R)
                shakes++; //Anhadimos un movimiento de colita
            }
            break;
        case SHAKEIT_R:  // Caso: mover colita a la dcha (SHAKEIT_R)
            LATBbits.LATB8=0; //Enable = 0 (apagar motor de rueda)
            LATBbits.LATB9=1; //Enable = 1 (encender motor de rueda)
        
            OC1RS = 0; // Rueda quiera
            OC2RS = PR2 * VEL_ALTA; //Velocidad alta en la rueda

            if (duty[DUTY0] != 851){ //si no se encuentra en la posicion debida...
                objetivopwm[DUTY0]=851; //actualizar posicion objetivo
                reached--; // actualizar cantidad de servos en su posicion objetivo
            }
            if (reached != 5) restart_Timer4_movservos(); // Si la cantidad de servos en su posicion objetivo no es 5 => gestionar movimiento de servos
            estado_rutina_perro=SHAKEIT_L; //Estado siguiente: mover colita a la izda (SHAKEIT_L)
            break;

        default: // Termina la rutina canina
            flag_posicion_segura=1; // Activar el flag para llevar al brazo a una posicion segura
            flag_rutina_perro=0; // Desacivar flag que da lugar a la rutina canina 
            estado_rutina_perro=SIT; // Resetear estado al inicial

            //Deshabilitar motores de las ruedas (Enable = 0)
            LATBbits.LATB8=0;
            LATBbits.LATB9=0;
        
            // Velocidad: 0
            OC1RS = 0;
            OC2RS = 0;
            break;
    }
    
}