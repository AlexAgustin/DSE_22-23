/*
Contiene las lineas a mostrar en la pantalla, la variable que contendra el
texto a mostrar y la funcion necesaria para copiar datos de FLASH a RAM.

Autores: Alex Agustin y Amanda Sin
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "LCD.h"
#include "commons.h"

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
    {"CPU M: -- Un: --""\x0D\x0A"}, 
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
  fila1 = 2; // se muestra la tercera entrada de Ventana_LCD en la linea superior de la pantalla LCD
  fila2 = 3; // se muestra la caurta entrada de Ventana_LCD en la linea inferior de la pantalla LCD
  *dir="--En curso... --""\x0D\x0A"; // se muestra en la pantalla del PC que ya puede manejarse el brazo
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