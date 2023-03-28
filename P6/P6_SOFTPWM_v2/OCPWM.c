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

unsigned int DUTY_MIN=(PR20ms/20) * MINPWM;	// valor minimo y maximo de DUTY. Se calculan 
unsigned int DUTY_MAX=(PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int flag_DUTY = 1;  // DUTY se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = 1; //cuando Duty cambie, se haga la conversion para visualizarlo en la LCDs

/*void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC
    
    OC1R =  (DUTY_MAX+DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (1,3ms))
    OC1RS = OC1R;               // inicializar registro secundario

    //conversion_ADC(&Ventana_LCD[0][posduty],OC1R);
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM
}*/

void visualizar_Duty(){
    conversion_4dig(&Ventana_LCD[0][posduty],duty);  // Guardar valor de duty en Ventana_LCD para su visualizacion en la pantalla
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}


void inic_PWM(){
    estado_PWM=PWM_ACTIVE; //Definir estado inicial
    duty = (DUTY_MAX+DUTY_MIN)/2; // Inicializar pulso con duracion intermedia (1,3ms))
    TRISDbits.TRISD0 = 0; //Definir como salida el pin que se usara para la se�al PWM
}
