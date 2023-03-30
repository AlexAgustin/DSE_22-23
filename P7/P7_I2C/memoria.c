/*
Contiene las lineas a mostrar en la pantalla, la variable que contendra el
texto a mostrar y la funcion necesaria para copiar datos de FLASH a RAM.

Autores: Alex Agustin y Amanda Sin
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"

//Lineas a mostrar
const unsigned char Mens_LCD_1[]={"- DSE 2022-23 --"};  //linea 1.1, 16 caracteres
const unsigned char Mens_LCD_2[]={"--- Pulsa S3  --"};  //linea 2.1, 16 caracteres
const unsigned char Mens_LCD_3[]={"---  I2C_v1  ---"};  //linea 1.2, 16 caracteres
const unsigned char Mens_LCD_4[]={"-- S4 comienza--"};  //linea 2.2, 16 caracteres
const unsigned char Mens_LCD_5[]={"P:0000 dist:0000"};  //linea 1.3, 16 caracteres
const unsigned char Mens_LCD_6[]={"Crono: 00:00,0  "};  //linea 2.3, 16 caracteres

unsigned char Ventana_LCD[2][16];

void copiar_FLASH_RAM (const unsigned char *texto, unsigned int i)
{
  unsigned int j;
  for (j=0;j<16;j++) Ventana_LCD[i][j]=texto[j]; //Copiar datos de FLASH a RAM
}

