/* 
Contiene las funciones asociadas al modulo T9 y 
a las esperas de un determinado numero de milisegundos y microsegundos
*/
#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "utilidades.h"
#include "LCD.h"
int inicializar_crono = 0;
void cronometro();

// inicializacion del timer 9
void inic_Timer9(unsigned long ciclos){
    if (ciclos < 65535) {
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
    unsigned long ciclos = (unsigned long)ms * (unsigned long)40000;
    
    if(ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA0=!LATAbits.LATA0; // Se conmuta el LED D3 (RA0)
    }
}

//Espera un determinado numero de microsegundos
void Delay_us(unsigned int us){
    unsigned long ciclos = (unsigned long)us * (unsigned long)40;
    
    if (ciclos < 16776960){// 65535 * 256. Valor maximo que puede aceptar el temporizador con el mayor prescaler
        inic_Timer9(ciclos);// inicializa el T9
        
        while(!IFS3bits.T9IF); // espera a que el temporizador indique que ha finalizado
        IFS3bits.T9IF = 0; // se marca la interrupcion como atendida
        T9CONbits.TON = 0; // apagar el temporizador
    }else{ // Valor de tiempo de espera superior a lo contemplado
        LATAbits.LATA2=!LATAbits.LATA2; // Se conmuta el LED D5 (RA2)
    }
}

void inic_Timer7 ()
{
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
    
    T7CONbits.TON = 1;	// el timer empieza en estado apagado
}	
unsigned int mili,deci,seg,dec,min;
void _ISR_NO_PSV _T7Interrupt()
{
    mili +=10; //se suman 10 milesimas de segundo
    IFS3bits.T7IF = 0;      // Puesta a 0 del flag IF del temporizador 7
}


void inic_crono()	
// inicializacion de las variables del cronometro: 
// milesimas de segundo (mili), decimas de segundo (deci), segundos (seg), decenas de segundos (dec), minutos (min)
{
	mili=0;
    deci=0;
    seg=0;
    dec=0;
    min=0;
    
    copiar_FLASH_RAM((unsigned char*) Mens_LCD_6, 1); //Copiar a memoria RAM la segunda linea
    
    line_2(); //nos posicionamos en la segunda linea
    puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea, desde RAM
}


void cronometro()	
// control del tiempo: espera 10 ms y luego actualiza
// inicializar cronometro: si el flag inicializar_crono esta activado, inicializa el cronometro
{
    if(inicializar_crono)
    {
        //el flag inicializar_crono esta activado
        inic_crono(); //inicializa el cronometro
        inicializar_crono = 0; //puesta a 0 del flag inicializar_crono
    }

  // actualiza las variables del cronometro y modifica los leds segun corresponda
    if (mili>=100){ //cada 100 milesimas de segundo
        deci+=1; //se suma una decima
        mili-=100; //reset milesimas
        LATAbits.LATA0=!LATAbits.LATA0; //conmutar LED D3
        conversion_tiempo(&Ventana_LCD[1][posds],deci*10); //asignacion del valor de las decimas de segundo
        Ventana_LCD[1][posds+1]=' ';
        line_2(); //nos posicionamos en la segunda linea
        puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea

        if (deci>=10){ //cada 10 decimas de segundo
            seg+=1; //se suma 1 seg
            deci-=10; //reset decimas
            LATAbits.LATA2=!LATAbits.LATA2; //conmuntar LED D5
            conversion_tiempo(&Ventana_LCD[1][posseg],seg); //asignacion del valor de los segundos
            line_2(); //nos posicionamos en la segunda linea
            puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea
            
            if (seg>=60){ //cada vez que pasen 6 decenas de segundo
                min+=1; //se suma 1 minuto
                seg-=60; //reset decenas
                LATAbits.LATA6=!LATAbits.LATA6; //conmuntar LED D9
                conversion_tiempo(&Ventana_LCD[1][posmin],min); //asignacion del valor de los minutos
                line_2(); //nos posicionamos en la segunda linea
                puts_lcd((unsigned char*) Ventana_LCD[1], 16); // Llevar al LCD la segunda linea
            }
        }
    }
}