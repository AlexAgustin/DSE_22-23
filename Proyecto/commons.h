// definiciones generales 
//=========================================================
#define Nop()   __builtin_nop()

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))


// definiciones para la pantalla LCD
//=========================================================

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

// definiciones: filas y posiciones de valores recogidos
//========================================================= 
#define NFILAS 23       //Numero de filas a mostrar
#define NCOLUMNAS 18    //Numero de columnas a mostrar por cada fila

//Filas
#define filacrono 3     // fila cronometro
#define filapoten 5     // fila potenciometro
#define filatemp 6      // fila temperatura
#define filax 7         // fila coordenada X
#define filay 8         // fila coordenada Y
#define filaz 9         // fila coordenada Z
#define filaduty01 11   // fila duty0 y duty1
#define filaduty23 12   // fila duty2 y duty3
#define filaduty4 13    // fila duty4
#define filaruedas 14   // fila ruedas
#define filacpu 15      // fila CPU
#define filacalibs0 17  // fila valores maximos y minimos de duty0
#define filacalibs1 18  // fila valores maximos y minimos de duty1
#define filacalibs2 19  // fila valores maximos y minimos de duty2
#define filacalibs3 20  // fila valores maximos y minimos de duty3
#define filacalibs4 21  // fila valores maximos y minimos de duty4
#define filadis 22      // fila distancia

//Posiciones generales
#define poscarac 15     // caracter presionado
#define pos4dig 12      // cifras 4 digitos
#define posdutyl 3      // duty a la izquierda
#define posdutyr 11     // duty a la derecha
#define poscpucur 6     // CPU: cont minimo
#define poscpumax 13    // CPU: cont maximo
#define poscalibmax 5   // duty maximo
#define poscalibmin 12  // duty minimo

// definiciones posiciones cronometro
#define posds 13    //decimas de segundo
#define poscs 14    //centesimas de segundo
#define posseg 10   //segundo
#define posmin 7    //minuto
 
// definiciones: filas y posiciones de valores recogidos
//========================================================= 
#define VERDUTY0 1
#define VERDUTY1 2
#define VERDUTY2 3
#define VERDUTY3 4
#define VERDUTY4 5
#define VERSOLOCALIB 8
#define VERCALIB 9
#define VERDUTYOC 10
#define VERDUTYALL 11
#define VERDUTYADC 12
#define VERDUTYOCADC 13
#define VERPERRO 14


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
#define PR20ms  12500 	// Periodo adecuado para conseguir 20 ms
                        // teniendo en cuenta un prescaler de 1:64.
                        // Fosc= 80 MHz Fcy = 40 MHz Tcy= 25 ns
#define MINPWM 0.5      // Duracion minima pulso PWM (en milisegundos)
#define MAXPWM 2.1      // Duracion maxima pulso PWM (en milisegundos)
#define SECURE_DUTY_0    951    //Duty0 en una posicion segura
#define SECURE_DUTY_1    1292   //Duty1 en una posicion segura
#define SECURE_DUTY_2    1292   //Duty2 en una posicion segura
#define SECURE_DUTY_3    466    //Duty3 en una posicion segura
#define SECURE_DUTY_4    312    //Duty4 en una posicion segura
#define VEL_ALTA    0.8  // Velocidad alta
#define VEL_BAJA    0.6  // Velocidad baja
#define DEF_DUTY_MIN (PR20ms/20) * MINPWM	// valor minimo y maximo por defecto de duty. Se calculan 
#define DEF_DUTY_MAX (PR20ms/20) * MAXPWM	// mediante los "define" PR20ms, MINPWM y MAXPWM

// Posiciones de duty en el array
#define DUTY0 0
#define DUTY1 1
#define DUTY2 2
#define DUTY3 3
#define DUTY4 4

// Definiciones relacionadas con el bus I2C
//==========================================
#define newdirsI2C 0xFC //Direccion a asignar al sensor
#define REG_COM 0 //Registro de comandos
#define REG_DATA 2 //Registro de datos
#define CHOQUE 5  // distancia de riesgo de choque




