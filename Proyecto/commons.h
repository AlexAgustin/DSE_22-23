// definiciones generales 
//=========================================================
#define Nop()   __builtin_nop()

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))


// definiciones para la pantalla LCD
//=========================================================

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



// definiciones posiciones cronometro
//=========================================================
#define posds 13 //decimas de segundo
#define poscs 14 //centesimas de segundo
#define posseg 10 //segundo
#define posmin 7 //minuto


// definiciones: filas y posiciones de valores recogidos
//========================================================= 
#define nfilas 19
#define ncolumnas 18

//Filas
#define filacrono 3 // fila cronometro
#define filapoten 5 // fila potencia
#define filatemp 6 // fila temperatura
#define filax 7 // fila coordenada X
#define filay 8 // fila coordenada Y
#define filaz 9 // fila coordenada Z
#define filaduty01 11 // fila duty0 y duty1
#define filaduty23 12 // fila duty2 y duty3
#define filaduty4 13 // fila duty4
#define filaruedas 14 // fila ruedas
#define filadutymin 18 //fila min duty
#define filadutymax 19 //fila max duty
#define filadis 20 //fila distancia

//Posiciones
#define poscarac 15 // caracter presionado
#define pos4dig 12 // cifras 4 digitos
#define pos3dig 13 // cifras de 3 digitos
#define posdutyl 3 // duty a la izquierda
#define posdutyr 11 // duty a la derecha

 
// Definiciones ligadas a la velocidad de transmision de UART2
//========================================================= 
#define Fosc 80000000        		// Frecuencia de reloj de la CPU (oscillator) 80MHz
#define Fcy	 Fosc/2		// Velocidad de ejecucion de las instrucciones
#define BAUDRATE2 57600  		// Velocidad de transmision de UART2 (baudios)
#define T_1BIT_US (1000000/BAUDRATE2)+1 // Duracion de 1 bit 

#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1	// valor U2BRG si BRGH=1
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1	// valor U2BRG si BRGH=0

 
// Definiciones relacionadas con la pantalla del terminal del PC
//========================================================= 
#define clrscr "\x1b[2J" //4 character: \x1b, [, 2, J
#define home "\x1b[H"    //3 character: \x1b, [, H

#define CR 0x0D		// carriage return
#define LF 0x0A		// line feed

 
// Definiciones de las entradas analogicas
//========================================================= 
#define potenciometro 5
#define termometro 4
#define coordx 0
#define coordy 1
#define coordz 2

// Definiciones relacionadas con el numero de muestras a tomar
//========================================================= 
#define INDIV_MUESTRAS 8    // Numero de muestras de cada entrada
#define NUM_ASIG 5          // Numero de entradas analogicas que se muestrean
#define TOTAL_MUESTRAS (INDIV_MUESTRAS * NUM_ASIG)   //Total de muestras que se tomaran cada vez


// Definiciones relacionadas con los servos y PWM
//========================================================= 
// Caso: servos del brazo
#define PR20ms  12500 	// Periodo adecuado para conseguir 20 ms
                        // teniendo en cuenta un prescaler de 1:64.
                        // Fosc= 80 MHz Fcy = 40 MHz Tcy= 25 ns
#define MINPWM 0.5      // Duracion minima pulso PWM (en milisegundos)
#define MAXPWM 2.1      // Duracion maxima pulso PWM (en milisegundos)
#define SECURE_DUTY (PR20ms/20) * (MINPWM+MAXPWM)/2    //Duty para una posicion segura

// Caso: ruedas
#define PR100us  4000 	// Periodo adecuado para conseguir 0,1 ms (100us)
                        // teniendo en cuenta un prescaler de 1:1.
                        // Fosc= 80 MHz Fcy = 40 MHz Tcy= 25 ns
#define MINOCPWM 0.0025      // Duracion minima pulso PWM (en milisegundos)
#define MAXOCPWM 0.0105      // Duracion maxima pulso PWM (en milisegundos)


// Definiciones relacionadas con el bus I2C
//==========================================
#define newdirsI2C 0xFE //Direccion a asignar al sensor
#define REG_COM 0 //Registro de comandos
#define REG_DATA 2 //Registro de datos




