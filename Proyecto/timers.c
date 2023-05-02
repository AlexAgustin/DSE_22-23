/* 
Por un lado, contiene las funciones asociadas al modulo T9 (inicializacion) y 
a las esperas de un determinado numero de milisegundos y microsegundos.

Ademas, contiene las funciones asociadas al modulo T7 (inicializacion y rutina de atencion) 
y a su correspondiente cronometro (inicializacion y gestion del paso del tiempo).

Por otro lado, contiene la funcion para inicializar el modulo T5 y su correspondiente rutina de atencion.

Ademas, contiene la funcion para inicializar el modulo T3.

Por otro lado, contiene las funciones asociadas al modulo T2 (inicializacion y rutina de atencion).
 
Ademas, contiene las funciones asociadas al modulo T6 (inicializacion, rutina de atencion y puesta en marcha) 

Autores: Alex Agustin y Amanda Sin
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "utilidades.h"
#include "LCD.h"
#include "ADC1.h"
#include "OCPWM.h"
#include "timers.h"
#include "i2c_funciones.h"
#include "srf08.h"

void cronometro();

int sum=0; 

//Variables globales
int inicializar_crono = 0;
unsigned int mili,deci,seg,min;


// inicializacion del timer 9
void inic_Timer9(unsigned long ciclos){
    TMR9 = 0 ; 	// Inicializar el registro de cuenta
    if (ciclos < 65535) { //65535 es el maximo numero de ciclos que entra en un tamanho de 2^16 (tamanho de PR) 
         T9CONbits.TCKPS = 0;	// escala del prescaler 00
         PR9 =  ciclos-1 ;	// Periodo del timer con prescaler 00
    }else{
        if ((ciclos/8) < 65535){
            T9CONbits.TCKPS = 1;	// escala del prescaler 01
            PR9 =  (ciclos/8)-1 ;	// Periodo del timer con prescaler 01
        }else{
            if ((ciclos/64) < 65535){
                T9CONbits.TCKPS = 2;	// escala del prescaler 10
                PR9 =  (ciclos/64)-1 ;	// Periodo del timer con prescaler 10
            }else{
                if ((ciclos/256) < 65535) {
                    T9CONbits.TCKPS = 3;	// escala del prescaler 11
                    PR9 =  (ciclos/256)-1 ;	// Periodo del timer con prescaler 11
                }
            }
        }
    }
    T9CONbits.TCS = 0;	// reloj interno
    T9CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    IFS3bits.T9IF = 0;      // Puesta a 0 del flag IF del temporizador 9
    T9CONbits.TON = 1;	// puesta en marcha del timer
}

//Espera un determinado numero de milisegundos
void Delay_ms(unsigned int ms){
    unsigned long ciclos = (unsigned long)ms * (unsigned long)40000; //el numero de ciclos equivale a ms * 40000
    
    if(ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA4=!LATAbits.LATA4; // Se conmuta el LED D7 (RA4)
        while(1); //espera infinita para advertir del error
    }
}

//Espera un determinado numero de microsegundos
void Delay_us(unsigned int us){
    unsigned long ciclos = (unsigned long)us * (unsigned long)40; //el numero de ciclos equivale a us * 40
    
    if (ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA5=!LATAbits.LATA5; // Se conmuta el LED D8 (RA5)
        while(1); //espera infinita para advertir del error
    }
}

void inic_Timer7 ()
{
    //Inicializar modulo T7
    TMR7 = 0 ; 	// Inicializar el registro de cuenta
    PR7 =  50000-1 ;	// Periodo del timer
		// Queremos que cuente 10 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 10 ms se necesitan 400.000 ciclos.
    T7CONbits.TCKPS = 1;	// escala del prescaler 01
    T7CONbits.TCS = 0;	// reloj interno
    T7CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC3bits.T7IE = 1;      // habilitacion de la interrupcion general de T7
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
    
    T7CONbits.TON = 1;	// encender el timer
}	



void _ISR_NO_PSV _T7Interrupt()
{
    //Han pasado 10 milesimas de segundo
    mili +=10; //se suman 10 milesimas de segundo
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
}


void inic_crono()	
// Inicializacion de las variables del cronometro: 
// milesimas de segundo (mili), decimas de segundo (deci), segundos (seg) y minutos (min)
{
	mili=0;
    deci=0;
    seg=0;
    min=0;
}


void cronometro()	
// control del tiempo (se actualiza cada 10 ms)
// inicializar cronometro: si el flag inicializar_crono esta activado, inicializa el cronometro
{
    if(inicializar_crono)
    {
        //el flag inicializar_crono esta activado
        //Puesta a 0 de las variables del cronometro
        inic_crono(); //inicializa el cronometro
        
        LATA=LATA & 0xff00; 	// Apagar los leds

        //Puesta a 0 de lo mostrado en la pantalla
        conversion_tiempo(&Ventana_LCD[filacrono][posmin],min); //asignacion del valor de los minutos en la posicion correspondiente de la linea a mostrar en pantalla  
        conversion_tiempo(&Ventana_LCD[filacrono][posseg],seg); //asignacion del valor de los segundos en la posicion correspondiente de la linea a mostrar en pantalla
        conversion_tiempo(&Ventana_LCD[filacrono][posds], deci); //asignacion del valor de las decimas de segundo en la posicion correspondiente de la linea a mostrar en pantalla
        Ventana_LCD[filacrono][poscs]=' '; // Poner un ' ' en la posicion asociada a las centesimas de segundo
        
        inicializar_crono = 0; //puesta a 0 del flag inicializar_crono
    }

    // actualiza las variables del cronometro y modifica el texto en pantalla y los leds segun corresponda
    if (mili>=100){ //cada 100 milesimas de segundo
        deci+=1; //se suma una decima
        mili-=100; //reset milesimas
        LATAbits.LATA0=!LATAbits.LATA0; //conmutar LED D3

        if (deci>=10){ //cada 10 decimas de segundo
            Nop();//Comprobacion de cuantas conversiones del ADC se han hecho
            Nop();
            num_conversiones=0; // resetear numero de conversiones
            seg+=1; //se suma 1 segundo
            deci-=10; //reset decimas
            LATAbits.LATA2=!LATAbits.LATA2; //conmuntar LED D5
            
            if (seg>=60){ //cada vez que pasen 60 segundos
                min+=1; //se suma 1 minuto
                seg-=60; //reset segundos
                LATAbits.LATA6=!LATAbits.LATA6; //conmuntar LED D9
                conversion_tiempo(&Ventana_LCD[filacrono][posmin],min); //asignacion del valor de los minutos en la posicion correspondiente de la linea a mostrar en pantalla  
            }
            conversion_tiempo(&Ventana_LCD[filacrono][posseg],seg); //asignacion del valor de los segundos en la posicion correspondiente de la linea a mostrar en pantalla
        }
        conversion_tiempo(&Ventana_LCD[filacrono][posds], deci*10); //asignacion del valor de las decimas de segundo en la posicion correspondiente de la linea a mostrar en pantalla
        Ventana_LCD[filacrono][poscs]=' '; // Poner un ' ' en la posicion asociada a las centesimas de segundo

    }
}

void inic_Timer5 ()
{
    //Inicializar modulo T5
    TMR5 = 0 ; 	// Inicializar el registro de cuenta
    PR5 =  12499 ;	// Periodo del timer ((2.5*40000)/8)-1
		// Queremos que cuente 2.5 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 2.5 ms se necesitan 100.000 ciclos.
    T5CONbits.TCKPS = 1;	// escala del prescaler 01
    T5CONbits.TCS = 0;	// reloj interno
    T5CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC1bits.T5IE = 1;      // habilitacion de la interrupcion general de T5
    IFS1bits.T5IF = 0;      // Puesta a 0 del flag IF del temporizador 5
    
    T5CONbits.TON = 1;	// encender el timer
}	

void _ISR_NO_PSV _T5Interrupt()
{
    static unsigned int estado_LCD = LCD_LINE1;
    static int i = 0;
    switch (estado_LCD){
        case LCD_LINE1: //Posicionamiento en la primera linea
            lcd_cmd(0x80);  	// Set DDRAM address (@0)
            estado_LCD = LCD_DATA1; // Cambio de estado, suguiente: envio de datos de la primera linea
            break;
        case LCD_DATA1: // Envio de los datos de la primera linea
            lcd_data(Ventana_LCD[fila1][i]); //Primero se manda uno de los datos de ventana
            i++; // se incrementa la posicion
            if (i == 16) {
                i=0; //Si es la ultima iteracion, se vuelve a la posicion 0 y...
                estado_LCD = LCD_LINE2; // y cambiamos de estado, siguiente: posicionamiento en la linea 2
            }
            break;
        case LCD_LINE2: //Posicionamiento en la segunda linea
            lcd_cmd(0xC0);  	// Set DDRAM address (@40)
            estado_LCD = LCD_DATA2; // Cambio de estado, suguiente: envio de datos de la segunda linea
            break;
        case LCD_DATA2: // Envio de los datos de la segunda linea
            lcd_data(Ventana_LCD[fila2][i]); //Primero se manda uno de los datos de ventana
            i++; // se incrementa la posicion
            if (i == 16) {
                i=0; //Si es la ultima iteracion, se vuelve a la posicion 0 y...
                estado_LCD = LCD_LINE1; // y cambiamos de estado, siguiente: posicionamiento en la linea 1
            }
            break;
        default:
            break; //Inalcanzable
    }
    
    IFS1bits.T5IF = 0;      // Puesta a 0 del flag IF del temporizador 5
}

void inic_Timer3 ()
{
    //Inicializar modulo T3
    TMR3 = 0 ; 	// Inicializar el registro de cuenta
    PR3 =  40000-1 ;	// Periodo del timer
		// Queremos que cuente 1 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 1 ms se necesitan 40.000 ciclos.
    T3CONbits.TCKPS = 0;	// escala del prescaler 00
    T3CONbits.TCS = 0;	// reloj interno
    T3CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC0bits.T3IE = 0;      // deshabilitar la interrupcion general de T3
    IFS0bits.T3IF = 0;      // Puesta a 0 del flag IF del temporizador 3
    
    T3CONbits.TON = 1;	// encender el timer
}


void inic_Timer2_OC1(){
    //Inicializar modulo T2
    TMR2 = 0; // Inicializar el registro de cuenta
    PR2 = 4000-1;	// Periodo del timer
		// Queremos que cuente 0,1 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 0,1 ms se necesitan 4.000 ciclos.
    
    T2CONbits.TCKPS = 0;	// escala del prescaler 1:1
    T2CONbits.TCS = 0;	// reloj interno
    T2CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC0bits.T2IE = 0;      // deshabilitar la interrupcion general de T2
    IFS0bits.T2IF = 0;      // Puesta a 0 del flag IF del temporizador 2
    
    T2CONbits.TON = 1;	// encender el timer
}


void inic_Timer4_OC3(){
    //Inicializar modulo T4
    TMR4 = 0; // Inicializar el registro de cuenta
    PR4 = 4000-1;	// Periodo del timer
		// Queremos que cuente 0,1 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
		// Para contar 0,1 ms se necesitan 4.000 ciclos.
    
    T4CONbits.TCKPS = 0;	// escala del prescaler 1:1
    T4CONbits.TCS = 0;	// reloj interno
    T4CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC1bits.T4IE = 0;      // deshabilitar la interrupcion general de T4
    IFS1bits.T4IF = 0;      // Puesta a 0 del flag IF del temporizador 4
    
    T4CONbits.TON = 1;	// encender el timer
}



void inic_Timer8_PWM(){
    //Inicializar modulo T8
    TMR8 = 0; // Inicializar el registro de cuenta
    PR8 =  12500-1 ;	// Periodo del timer
        // Inicialmente queremos que cuente 20 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
        // Para contar 20 ms se necesitan 800.000 ciclos.
		// Posteriormente, el valor de PR8 ira cambiando.
    
    T8CONbits.TCKPS = 2;	// escala del prescaler 1:64
    T8CONbits.TCS = 0;	// reloj interno
    T8CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC3bits.T8IE = 1;      // habilitar la interrupcion general de T8
    IFS3bits.T8IF = 0;      // Puesta a 0 del flag IF del temporizador 8
    
    T8CONbits.TON = 1;	// encender el timer
}

void _ISR_NO_PSV _T8Interrupt(){
    switch (estado_PWM){
        case PWM0_ACTIVE: //Activar PWM (0)
            LATDbits.LATD8 = 1; // Puesta a 1 (RE10)
            PR8 = duty0; //Determinar periodo del temporizador 8 a partir de duty0
            sum=duty0; // Guardar ciclos transcurridos
            estado_PWM = PWM1_ACTIVE; // Estado siguiente: activar PWM (1)
            break;
        case PWM1_ACTIVE: //Activar PWM (1)
            LATDbits.LATD8 = 0; // Puesta a 0 (RE10)
            LATDbits.LATD9 = 1; // Puesta a 1 (RE11)
            PR8 = duty1; //Determinar periodo del temporizador 8 a partir de duty1
            sum+=duty1; // Acumular ciclos transcurridos
            estado_PWM = PWM2_ACTIVE; // Estado siguiente: senhales desactivadas
            break;
        case PWM2_ACTIVE: //Activar PWM (2)
            LATDbits.LATD9 = 0; // Puesta a 0 (RE11)
            LATDbits.LATD10 = 1; // Puesta a 1 (RE12)
            PR8 = duty2; //Determinar periodo del temporizador 8 a partir de duty2
            sum+=duty2; // Acumular ciclos transcurridos
            estado_PWM = PWM3_ACTIVE; // Estado siguiente: senhales desactivadas
            break;
        case PWM3_ACTIVE: //Activar PWM (3)
            LATDbits.LATD10 = 0; // Puesta a 0 (RE12)
            LATDbits.LATD11 = 1; // Puesta a 1 (RE13)
            PR8 = duty3; //Determinar periodo del temporizador 8 a partir de duty3
            sum+=duty3; // Acumular ciclos transcurridos
            estado_PWM = PWM4_ACTIVE; // Estado siguiente: senhales desactivadas
            break;
        case PWM4_ACTIVE: //Activar PWM (4)
            LATDbits.LATD11 = 0; // Puesta a 0 (RE13)
            LATDbits.LATD12 = 1; // Puesta a 1 (RE14)
            PR8 = duty4; //Determinar periodo del temporizador 8 a partir de duty4
            sum+=duty4; // Acumular ciclos transcurridos
            estado_PWM = PWM_RESTO; // Estado siguiente: senhales desactivadas
            break;
        case PWM_RESTO: // Senhales desactivadas
            LATDbits.LATD12 = 0; // Puesta a 0 (RE14)
            PR8 = (PR20ms-sum); // Determinar periodo del temporizador 8 a partir de los ciclos restantes
            estado_PWM = PWM0_ACTIVE; // Estado siguiente: activar PWM (0)
            break;
        default:
            break;
    }
    IFS3bits.T8IF = 0;      // Puesta a 0 del flag IF del temporizador 8
}

void inic_Timer6(){
    //Inicializar modulo T6
    TMR6 = 0; // Inicializar el registro de cuenta
    PR6 =  43750-1 ;	// Periodo del timer
        // Inicialmente queremos que cuente 70 ms.
		// Fosc= 80 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 40 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy= 25 ns para ejecutar una instruccion
        // Para contar 70 ms se necesitan 2.800.000 ciclos.
    
    T6CONbits.TCKPS = 2;	// escala del prescaler 1:64
    T6CONbits.TCS = 0;	// reloj interno
    T6CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    
    IEC2bits.T6IE = 1;      // habilitar la interrupcion general de T6
    IFS2bits.T6IF = 0;      // Puesta a 0 del flag IF del temporizador 6
    
    T6CONbits.TON = 1;	// encender el timer T6
}

void _ISR_NO_PSV _T6Interrupt(){
    flag_dis = 1; //Poner a 1 el flag para realizar la medicion de la distancia
    T6CONbits.TON = 0;	// apagar el timer T6
    IFS2bits.T6IF = 0;      // Puesta a 0 del flag IF del temporizador 6
}


void restart_Timer6(){
    TMR6 = 0; // Inicializar el registro de cuenta
    T6CONbits.TON = 1;	// encender el timer T6
}


