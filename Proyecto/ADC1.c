/* Funciones para el modulo ADC1
================================================
 * Contiene las funciones de inicializacion del modulo ADC, comienzo del muestreo, 
 * tratamiento de valores (obtener medias y guardarlas en las posiciones debidas de Ventana_LCD) y la rutina de atencion.
Autores: Alex y Amanda
Fecha: Marzo 2023
*/

#include <p24HJ256GP610A.h>
#include "commons.h"
#include "utilidades.h"
#include "memoria.h"
#include "OCPWM.h"
#include "timers.h"

unsigned int Poten_value[8];
unsigned int Temp_value[8];
unsigned int X_value[8];
unsigned int Y_value[8];
unsigned int Z_value[8];
int flag_ADC =0;
unsigned long num_conversiones=0;

void inic_ADC1 (void) // inicializacion del modulo
{
// Inicializacion registro control AD1CON1
AD1CON1 = 0;       // todos los campos a 0

// Salida de 12 bits o 10 bits
//AD1CON1bits.AD12B = 0;  

// Comienzo digitalizacion automatico
// 111=Auto-convert / 010=TMR3 ADC1 y TMR5 ADC2 / 001=INT0 / 000= SAMP 
AD1CON1bits.SSRC= 2;  //Funciona con T3	

// Muestreo simultaneo o secuencial
//AD1CON1bits.SIMSAM = 0; 
             
// Comienzo muestreo automatico o por programa (SAMP=1) 		
AD1CON1bits.ASAM = 1;

                    
// Inicializacion registro control AD1CON2
AD1CON2= 0 ;  // todos los campos a 0


// Inicializacion registro control AD1CON3
AD1CON3=0;    // todos los campos a 0
// Reloj con el que funciona el ADC:  0= reloj CPU; 1= RC erlojua 
//AD1CON3bits.ADRC = 0;  // 
AD1CON3bits.SAMC=31;   // Tiempo muestreo = numero de Tad 
AD1CON3bits.ADCS=3;   // Relacion entre TAD y Tcy TAD = Tcy(ADCS+1)


// Inicializacion registro control AD1CON4
AD1CON4=0; //relacionado con DMA, no lo usaremos


// Inicializacion registro AD1CHS123
AD1CHS123=0;	//seleccion del canal 1,2 y 3


// Inicializacion registro AD1CHS0
AD1CHS0=0; 
AD1CHS0bits.CH0SA=5; // elige la entrada analogica conectada

//AD1CHS0bits.CH0SB = 0;


// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0

AD1CSSH = 0;   // 16-31 
AD1CSSL = 0;   // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales
// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5=0;   // potenciometro
AD1PCFGLbits.PCFG4=0;   // sensor temperatura
AD1PCFGLbits.PCFG0=0;   // coordenada X
AD1PCFGLbits.PCFG1=0;   // coordenada Y
AD1PCFGLbits.PCFG2=0;   // coordenada Z


// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF=0;  // Puesta a 0 del flag IF  
IEC0bits.AD1IE=1; //trabajaremos por interrupcion   
//IPC3bits.AD1IP=xx;    

//AD1CON
AD1CON1bits.ADON=1;  // Habilitar el modulo ADC
}



// Funcion que calcula la media de las muestras tomadas y prepara los datos para ser visualizados
void tratar_valorADC1 () 
{   
    float Poten_media = 0, Temp_media=0, X_media=0, Y_media=0, Z_media=0;
    int i;

    // Calcular la media de la potencia, temperatura, coordenada x y coordenada y
    for(i=0; i<INDIV_MUESTRAS; i++) //Sumar los valores recogidos de cada entrada analogica
    {
        Poten_media += Poten_value[i]; //potencia
        Temp_media += Temp_value[i]; //temperatura
        X_media += X_value[i]; //coordenada x
        Y_media += Y_value[i]; //coordenada y
        Z_media += Z_value[i]; //coordenada z
    }
    
    //Dividir la suma realizada por el numero de muestras tomadas
    Poten_media = Poten_media / INDIV_MUESTRAS; 
    Temp_media = Temp_media / INDIV_MUESTRAS;
    X_media = X_media / INDIV_MUESTRAS;
    Y_media = Y_media / INDIV_MUESTRAS;
    Z_media = Z_media / INDIV_MUESTRAS;
    
    
    OC1RS = (X_media/999) * (OC_DUTY_MAX - OC_DUTY_MIN) + OC_DUTY_MIN; // Obtener OC1RS a partir de la coordenada X
    OC2RS = (Y_media/999) * (OC_DUTY_MAX - OC_DUTY_MIN) + OC_DUTY_MIN; // Obtener OC2RS a partir de la coordenada Y

    if(!flag_DUTY){ //Si flag_DUTY==0 se obtiene duty0..4  a partir de la potencia
        duty[DUTY0]  = (Poten_media/1023) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN; // Obtener duty[DUTY0]  a partir de la potencia
        duty[DUTY1] = (Temp_media/999) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN; // Obtener duty1 a partir de la temperatura
        duty[DUTY2] = (X_media/999) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN; // Obtener duty2 a partir de la coordenada X
        duty[DUTY3] = (Y_media/999) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN; // Obtener duty3 a partir de la coordenada Y
        duty[DUTY4] = (Z_media/999) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN; // Obtener duty[DUTY4] a partir de la coordenada Z
        flag_Duty_LCD = VERDUTYALL; // Poner a 6 el flag para guardar los nuevos valores de duty[0-4], OC1RS y OC2RS en Ventana_LCD para su visualizacion en la pantalla
    } else {  
        flag_Duty_LCD = VERDUTYOC; // Poner a 7 el flag para guardar los nuevos valores de OC1RS y OC2RS en Ventana_LCD para su visualizacion en la pantalla
    }

    //Escribir el valor de cada dato a visualizar en la posicion correspondiente de Ventana_LCD
    conversion_4dig(&Ventana_LCD[filapoten][pos4dig],Poten_media); 
    conversion_4dig(&Ventana_LCD[filatemp][pos4dig],Temp_media); 
    conversion_4dig(&Ventana_LCD[filax][pos4dig],X_media); 
    conversion_4dig(&Ventana_LCD[filay][pos4dig],Y_media);
    conversion_4dig(&Ventana_LCD[filaz][pos4dig],Z_media);
    
    flag_ADC=0; //Puesta a 0 del flag 
}

void _ISR_NO_PSV _ADC1Interrupt(){
    
    num_conversiones++;
    static int num_muestras=0;
    static int i = 0;
    if (!flag_ADC){ //Comprobacion de que el programa principal ha terminado con las muestras anteriores
        switch(AD1CHS0bits.CH0SA){ //Switch de las diferentes entradas analogicas
            case potenciometro: //Entrada del potenciometro
                Poten_value[i] = ADC1BUF0; //Se guarde el valor recogido en la posicion de la tabla de muestras que le corresponde
                AD1CHS0bits.CH0SA = termometro; //Se define la siguiente senhal a muestrear
                num_muestras++; //Incrementar el numero de muestras tomadas
                break;
            case termometro: //entrada del termometro
                Temp_value[i] = ADC1BUF0; //Se guarde el valor recogido en la posicion de la tabla de muestras que le corresponde
                AD1CHS0bits.CH0SA = coordx; //Se define la siguiente senhal a muestrear
                num_muestras++; //Incrementar el numero de muestras tomadas
                break;
            case coordx: //entrada de la coordenada X del joystick
                Nop();
                Nop();
                X_value[i] = ADC1BUF0; //Se guarde el valor recogido en la posicion de la tabla de muestras que le corresponde
                AD1CHS0bits.CH0SA = coordy; //Se define la siguiente senhal a muestrear
                num_muestras ++; //Incrementar el numero de muestras tomadas
                break;
            case coordy: //entrada de la coordenada Y del joystick
                Y_value[i] = ADC1BUF0; //Se guarde el valor recogido en la posicion de la tabla de muestras que le corresponde
                AD1CHS0bits.CH0SA = coordz; //Se define la siguiente senhal a muestrear
                num_muestras ++; //Incrementar el numero de muestras tomadas
                break;
            case coordz: //entrada de la coordenada Z del joystick
                Z_value[i] = ADC1BUF0; //Se guarde el valor recogido en la posicion de la tabla de muestras que le corresponde
                AD1CHS0bits.CH0SA = potenciometro; //Se define la siguiente senhal a muestrear
                num_muestras ++; //Incrementar el numero de muestras tomadas
                i++; //incrementar la posicion de la tabla de muestras (indice de escritura)
                break;
            default:
                break;
        }
        if(num_muestras==TOTAL_MUESTRAS){ // se han tomado todas las muestras requeridas
            flag_ADC=1; //Puesta a 1 del flag
            num_muestras=0; //Resetear num_muestras
            i=0; //Resetear indice de escritura
        }
    }
    //comienzo_muestreo(); // Comienza el muestreo y 31Tad despues comienza la digit
    IFS0bits.AD1IF=0; // Puesta a 0 del flag IF
}


