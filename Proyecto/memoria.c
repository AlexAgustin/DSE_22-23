/*
Contiene la variable que contendra el texto a mostrar en la pantalla
y la funcion para actualizar la informacion a mostrar en la
LCD una vez comienza el programa.

Autores: Alex Agustin y Amanda Sin
Fecha: mayo 2023
*/

#include "p24HJ256GP610A.h"
#include "LCD.h"
#include "commons.h"
#include "utilidades.h"

unsigned char Ventana_LCD[NFILAS][NCOLUMNAS] = {
    {"- DSE 2022-23 --""\x0D\x0A"},
    {"Joy calib/S4 com""\x0D\x0A"},
    {"=== Proyecto ===""\x0D\x0A"},
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
    {"S1 M:---- m:----""\x0D\x0A"},
    {"S2 M:---- m:----""\x0D\x0A"},
    {"S3 M:---- m:----""\x0D\x0A"},
    {"S4 M:---- m:----""\x0D\x0A"},
    {"S5 M:---- m:----""\x0D\x0A"},
    {"Distancia:  ----""\x0D\x0A"}
};


void actualizar_Ventana_LCD ()
// Actualiza la informacion a mostrar en la LCD: titulo y crono
{
  fila1 = 2; // se muestra la tercera entrada de Ventana_LCD en la linea superior de la pantalla LCD (titulo)
  fila2 = 3; // se muestra la caurta entrada de Ventana_LCD en la linea inferior de la pantalla LCD (crono)
}

