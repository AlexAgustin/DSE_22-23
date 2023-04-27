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

unsigned int DUTY_MIN=(PR20ms/20) * MINPWM;	// valor minimo y maximo de duty. Se calculan 
unsigned int DUTY_MAX=(PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int flag_DUTY = 1;  // duty0 se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = 1; //cuando duty0 cambia, se hace la conversion para visualizarlo en la pantalla  (flag a 1)

unsigned int duty0;
unsigned int duty1;
unsigned int duty2;
unsigned int duty3;
unsigned int duty4;
unsigned int estado_PWM;

void visualizar_Duty(){
    if (flag_Duty_LCD==1)
        conversion_4dig(&Ventana_LCD[filaduty01][posdutyl],duty0);  // Guardar valor de duty0 en Ventana_LCD para su visualizacion en la pantalla
    else if (flag_Duty_LCD==2)
        conversion_4dig(&Ventana_LCD[filaduty01][posdutyr],duty1); // Guardar valor de duty1 en Ventana_LCD para su visualizacion en la pantalla
    else if (flag_Duty_LCD==3)
        conversion_4dig(&Ventana_LCD[filaduty23][posdutyl],duty2); // Guardar valor de duty2 en Ventana_LCD para su visualizacion en la pantalla
    else if (flag_Duty_LCD==4)
        conversion_4dig(&Ventana_LCD[filaduty23][posdutyr],duty3); // Guardar valor de duty3 en Ventana_LCD para su visualizacion en la pantalla
    else if (flag_Duty_LCD==5)
        conversion_4dig(&Ventana_LCD[filaduty4][posdutyl],duty4); // Guardar valor de duty4 en Ventana_LCD para su visualizacion en la pantalla
    
    
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}


void inic_PWM(){
    estado_PWM=PWM0_ACTIVE; //Definir estado inicial
    duty0 = (DUTY_MAX+DUTY_MIN)/2; // Inicializar pulso con duracion intermedia (1,3ms))
    duty1 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty2 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty3 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    duty4 = duty0; // Inicializar pulso con duracion intermedia (1,3ms))
    TRISDbits.TRISD0 = 0; //Definir como salida el pin que se usara para la senhal PWM (0)
    TRISDbits.TRISD1 = 0; //Definir como salida el pin que se usara para la senhal PWM (1)
    TRISDbits.TRISD2 = 0; //Definir como salida el pin que se usara para la senhal PWM (2)
    TRISDbits.TRISD3 = 0; //Definir como salida el pin que se usara para la senhal PWM (3)
    TRISDbits.TRISD4 = 0; //Definir como salida el pin que se usara para la senhal PWM (4)
}
