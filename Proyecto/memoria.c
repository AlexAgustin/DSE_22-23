/*
Contiene las lineas a mostrar en la pantalla, la variable que contendra el
texto a mostrar y la funcion necesaria para copiar datos de FLASH a RAM.

Autores: Alex Agustin y Amanda Sin
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "LCD.h"
#include "commons.h"

//Lineas a mostrar
/*const unsigned char Mens_LCD_1[]={"- DSE 2022-23 --"};  //linea 1.1, 16 caracteres
const unsigned char Mens_LCD_2[]={"--- Pulsa S3  --"};  //linea 2.1, 16 caracteres
const unsigned char Mens_LCD_3[]={"---  I2C_v1  ---"};  //linea 1.2, 16 caracteres
const unsigned char Mens_LCD_4[]={"-- S4 comienza--"};  //linea 2.2, 16 caracteres
const unsigned char Mens_LCD_5[]={"P:0000 dist:0000"};  //linea 1.3, 16 caracteres
const unsigned char Mens_LCD_6[]={"Crono: 00:00,0  "};  //linea 2.3, 16 caracteres*/

unsigned char Ventana_LCD[NFILAS][NCOLUMNAS] = {
    {"- DSE 2022-23 --""\x0D\x0A"},
    {"-- S4 comienza--""\x0D\x0A"},
    {"==== Titulo ====""\x0D\x0A"},
    {"Crono: 00:00,0  ""\x0D\x0A"},
    {"= INFOR PIC24H =""\x0D\x0A"},
    {"Potenciom:  ----""\x0D\x0A"},
    {"Temperatura: ---""\x0D\x0A"},
    {"Joystick X: ----""\x0D\x0A"},
    {"Joystick Y: ----""\x0D\x0A"},
    {"Joystick Z: ----""\x0D\x0A"},
    {"Dur. Pulsos  PWM""\x0D\x0A"},
    {"S1:---- S2:---- ""\x0D\x0A"},
    {"S3:---- S4:---- ""\x0D\x0A"},
    {"S5:----         ""\x0D\x0A"},
    {"VA:---- VD:---- ""\x0D\x0A"},
    {"CPU M: -- Un: --""\x0D\x0A"}, //M:-- Un:--EN WHILe inicializad temp y abajo esperas a if, cuentas tiempo de 20 ms 
    {"   = Calib =    ""\x0D\x0A"},
    {"S0 M:---- m:----""\x0D\x0A"},
    {"S1 M:---- m:----""\x0D\x0A"},
    {"S2 M:---- m:----""\x0D\x0A"},
    {"S3 M:---- m:----""\x0D\x0A"},
    {"S4 M:---- m:----""\x0D\x0A"},
    {"Distancia:  ----""\x0D\x0A"}
};

void actualizar_Ventana_LCD (unsigned char * dir)
{
  fila1 = 2;
  fila2 = 3;
  *dir="--En curso... --""\x0D\x0A";
}

void gestion_cont(unsigned long cont)
{
  static unsigned long contmin = ULLONG_MAX;
  static unsigned long contmax = ULLONG_MIN;

  // TODO: Calcular porcentaje sobre 100, need el otro datito


  if(cont<contmin){
    contmin = cont;
    conversion_2dig(&Ventana_LCD[filacpu][poscpumin], contmin); 
  } 
  else if (cont>contmax){
    contmax = cont;
    conversion_2dig(&Ventana_LCD[filacpu][poscpumax], contmax); 
  }
}