/*
Funciones relacionadas con el modulo OC para generar señal PWM

Contiene la funcion de inicializacion del modulo OC1

Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"

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
    conversion_4dig(&Ventana_LCD[0][posduty],OC1RS);
    flag_Duty_LCD = 0;
}

//anadir funcion de iniclaizacion: valor medio, estado, ...
