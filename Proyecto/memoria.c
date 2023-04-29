/*
Contiene las lineas a mostrar en la pantalla, la variable que contendra el
texto a mostrar y la funcion necesaria para copiar datos de FLASH a RAM.

Autores: Alex Agustin y Amanda Sin
Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "LCD.h"

//Lineas a mostrar
/*const unsigned char Mens_LCD_1[]={"- DSE 2022-23 --"};  //linea 1.1, 16 caracteres
const unsigned char Mens_LCD_2[]={"--- Pulsa S3  --"};  //linea 2.1, 16 caracteres
const unsigned char Mens_LCD_3[]={"---  I2C_v1  ---"};  //linea 1.2, 16 caracteres
const unsigned char Mens_LCD_4[]={"-- S4 comienza--"};  //linea 2.2, 16 caracteres
const unsigned char Mens_LCD_5[]={"P:0000 dist:0000"};  //linea 1.3, 16 caracteres
const unsigned char Mens_LCD_6[]={"Crono: 00:00,0  "};  //linea 2.3, 16 caracteres*/

unsigned char Ventana_LCD[20][18] = {
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
    {" X:----  Y:---- ""\x0D\x0A"},
    {"CPU M: -- Un: --""\x0D\x0A"},
    {"Joy pequ X: ----""\x0D\x0A"},
    {"Calib m S-: ----""\x0D\x0A"},
    {"      M S-: ----""\x0D\x0A"},
    {"Distancia:   ---""\x0D\x0A"}
};

void actualizar_Ventana_LCD ()
{
  fila1 = 2;
  fila2 = 3;
  //Ventana_LCD[1] = {"--En curso... --""\x0D\x0A"};
}

