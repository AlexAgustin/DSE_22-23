/*
Funciones relacionadas con el modulo OC1 para generar senhal PWM

Contiene la funcion de inicializacion del modulo OC1 y la funcion para visualizar duty en la pantalla.

Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"

unsigned int DUTY_MIN=(PR20ms/20) * MINPWM;	// valor minimo y maximo de duty. Se calculan
unsigned int DUTY_MAX=(PR20ms/20) * MAXPWM;	// mediante los "define" PR20ms, MINPWM y MAXPWM
unsigned int flag_DUTY = 1;  // duty se gestionara por defecto a traves de UART
unsigned int flag_Duty_LCD = 1; //cuando duty cambia, se hace la conversion para visualizarlo en la pantalla (flag a 1)

void inic_OC1 ()
{
    OC1CON=0; // Deshabilita modulo Output Compare
    
    //OC1CONbits.OCM=0b000;     // deshabilitar OC1 
    OC1CONbits.OCTSEL=0;      // seleccionar T2 para el OC (OCTSEL= 0 -> Selecciona T2 (1 -> T3))
    
    OC1R =  (DUTY_MAX+DUTY_MIN)/2; 		// Inicializar pulso con duracion intermedia (1,3ms)) (duty cycle para el primer pulso PWM)
    OC1RS = OC1R;               // inicializar registro secundario (duty cycle para el siguiente pulso PWM)
    
    OC1CONbits.OCM=0b110;       // habilitar OC1 en modo PWM sin prot
}

void visualizar_Duty(){
    conversion_4dig(&Ventana_LCD[0][posduty],OC1RS); // Guardar valor de duty en Ventana_LCD para su visualizacion en la pantalla
    flag_Duty_LCD = 0; //Puesta a 0 del flag para guardar duty en Ventana_LCD para su visualizacion en la pantalla
}
