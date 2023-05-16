/* Funciones para el modulo LCD
================================================
    -	Inicializacion
    -	Enviar un comando
    -	Enviar un dato

Autores: Alex y Amanda
Fecha: Mayo 2023
**********************************************************************/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers.h"

//filas de Ventana_LCD a mostrar en la LCD
unsigned int fila1, fila2;

/*****	LCD SUBROUTINES  *****/ 
//Subrutina para enviar un comando
void lcd_cmd (char cmd)        // subroutine for lcd commands
{
  RW = 0;             // RW=0, para escribir
  RS = 0;	      // RS=0, se va a escribir un comando
  DATA &= 0xFF00;     // pines RD0-RD7 (dato que se envia): poner a 0
  DATA |= cmd;        // copiar a esos pines el parametro cmd
  E = 1;              // E=1. Debe permanecer activado al menos 230 ns. 

  Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop(); //aseguramos el tiempo para que los datos esten estables
  
  E = 0;		// desactivar E
  RW = 1;		// desactivar escritura
}

//Subrutina para enviar un dato
void lcd_data (char data)      // subroutine for lcd data
{
  RW = 0;       	// RW=0, para escribir
  RS = 1;            	// RS=1, se va a escribir un caracter
  DATA &= 0xFF00;    	// pines RD0-RD7 (dato que se envia): poner a 0
  DATA |= data;      	// copiar a esos pines el parametro data
  E = 1;             	// E=1. Debe permanecer activado al menos 230 ns.

  Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop(); //aseguramos el tiempo para que los datos esten estables
  
  E = 0;             	// desactivar E
  RS = 0;            	// desactivar RS
  RW = 1;		// desactivar escritura
}

//Inicializacion del modulo LCD
void Init_LCD ()             // initialize LCD display
{
  // 15mS delay after Vdd reaches nnVdc before proceeding 
  // with LCD initialization
  // not always required and is based on system Vdd rise rate
  Delay_ms (15); // 15 ms delay
			
  /* set initial states for the data and control pins */
  DATA &= 0xFF00;	
  RW = 0;                       // R/W state set low
  RS = 0;                       // RS state set low
  E = 0;                        // E state set low

  /* set data and control pins to outputs */
  TRISDATA &= 0xFF00;
  RW_TRIS = 0;                  // RW pin set as output
  RS_TRIS = 0;                  // RS pin set as output
  E_TRIS = 0;                   // E pin set as output

  /* LCD initialization sequence */ 
  lcd_cmd (0x38);				// function set (3 veces)
  Delay_ms (5);         // 5 ms delay
  lcd_cmd (0x38);
  Delay_us (100);       // 100 us delay
  lcd_cmd (0x38);
  Delay_us (40);      // 40 us delay
  lcd_cmd (0x38);  
  Delay_us (40);      // 40 us delay
  lcd_cmd (0x0C);       // Display on/off control, cursor blink off (0x0C)
  Delay_us (40);      // 40 us delay
  lcd_cmd (0x06);	// Entry mode set (0x06)
  Delay_us (40);      // 40 us delay
  
  //Inicializar las filas a mostrar en la LCD
  fila1=0;
  fila2=1;
}