// definiciones generales 

#define Nop()   __builtin_nop()

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))

/////////////////////////////////////
// definiciones para la pantalla LCD
/////////////////////////////////////

/* 
   For Explorer 16 board, here are the data and control signal definitions
   RS -> RB15
   E  -> RD4
   RW -> RD5
   DATA -> RE0 - RE7   
*/

// Control signal data pins 
#define  RW  LATDbits.LATD5       // LCD R/W signal
#define  RS  LATBbits.LATB15      // LCD RS signal
#define  E   LATDbits.LATD4       // LCD E signal 

// Control signal pin direction 
#define  RW_TRIS	TRISDbits.TRISD5 
#define  RS_TRIS	TRISBbits.TRISB15
#define  E_TRIS		TRISDbits.TRISD4

// Data signals and pin direction
#define  DATA      LATE           // Port for LCD data
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE          // I/O setup for data Port


/////////////////////
// definiciones posiciones cronometro
//////////////////////
#define posds 13 //decimas de segundo
#define poscs 14 //centesimas de segundo
#define posseg 10 //segundo
#define posmin 7 //minuto

/////////////////////
// definiciones posiciones valores recogidos
// linea de visualizacion actual:
// {"P:0000 dist:0000"}
////////////////////// 
#define pospoten 2 //potencia
#define postemp 2 //temperatura
#define posx 2 // coordenada x
#define posy 2 // coordenada y
#define posduty 12 // duty
#define posdis 12 // distancia


// Definiciones ligadas a la velocidad de transmision de UART2

#define Fosc 80000000        		// Frecuencia de reloj de la CPU (oscillator) 80MHz
#define Fcy	 Fosc/2		// Velocidad de ejecucion de las instrucciones
#define BAUDRATE2 4800  		// Velocidad de transmision de UART2 (baudios)
#define T_1BIT_US (1000000/BAUDRATE2)+1 // Duracion de 1 bit 

#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1	// valor U2BRG si BRGH=1
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1	// valor U2BRG si BRGH=0

// Definiciones relacionadas con la pantalla del terminal del PC

#define clrscr "\x1b[2J" //4 character: \x1b, [, 2, J
#define home "\x1b[H"    //3 character: \x1b, [, H

#define CR 0x0D		// carriage return
#define LF 0x0A		// line feed

// Definiciones de las entradas analogicas
#define potenciometro 5
#define termometro 4
#define coordx 0
#define coordy 1

// Definiciones relacionadas con el numero de muestras a tomar
#define INDIV_MUESTRAS 8    // Numero de muestras de cada entrada
#define NUM_ASIG 4          // Numero de entradas analogicas que se muestrean
#define TOTAL_MUESTRAS (INDIV_MUESTRAS * NUM_ASIG)   //Total de muestras que se tomaran cada vez


// Definiciones relacionadas con los servos y PWM
//=========================================================  
#define PR20ms  12500 	// Periodo adecuado para conseguir 20 ms
                        // teniendo en cuenta un prescaler de 1:64.
                        // Fosc= 80 MHz Fcy = 40 MHz Tcy= 25 ns
#define MINPWM 0.5      // Duracion minima pulso PWM (en milisegundos)
#define MAXPWM 2.1      // Duracion maxima pulso PWM (en milisegundos)

// Definiciones relacionadas con el bus I2C
#define dirI2C 0xE2


