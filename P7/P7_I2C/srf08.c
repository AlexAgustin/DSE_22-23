/*
Funciones para utilizar el sensor de distancia.

Contiene las funciones para gesetionar la medicion de la distancia, 
iniciar la medicion, leer la medicion, 
cambiar la direccion y detectar la direccion.

Autoras/es: Alex y Amanda

Fecha: Marzo 2023
*/


#include "i2c_funciones.h"
#include "timers.h"
#include "commons.h"

unsigned int flag_dis = 0;


void gestion_dis(unsigned char dirI2C)
//Gestionar la medicion de la distancia
{
   unsigned int dis;
   dis = leer_medicion();  // Lectura de la distancia media
   conversion_4dig(&Ventana_LCD[0][posdis], dis); //Guardar distancia media en Ventana_LCD para su proyeccion en la pantalla
   inic_medicion_dis(); // Puesta en marcha de una nueva medicion
   restart_Timer6(); // Puesta en marcha del temporizador T6
   flag_dis = 0; // puesta a 0 del flag para medir la distancia
}

unsigned int inic_medicion_dis (unsigned char dirI2C)
 // Puesta en marcha de una medicion
 // Parametro dirI2C corresponde a la direccion I2C del sensor
{
   LDByteWriteI2C_1(dirI2C,0,0x51); //Mandar al registro 0 (comandos) del esclavo (dirI2C)
                                    //el comando para que de los resultados en cm (0x51).
}

unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) 
 // Funcion para leer la distancia media
 // Parametro dirI2C corresponde a la direccion I2C del sensor
 // Parametro dis proporciona la direccion de un array en el que devolver la medicion
 // Cada medicion 2 bytes
{
   unsigned int dis_media;
   LDByteReadI2C_1(dirI2C,2,dis,2); //Leer del registro 2 (distancia) del esclavo (dirI2C)
                                    //los dos bytes corresponientes a la distancia media.
   dis_media = (dis[0]<<8) || dis[1]; //Obtener la distancia media a partir de las dos posiciones del array dis
   return(dis_media);
}


void cambiar_direccion (unsigned char dirI2C, unsigned char newdirI2C)
 // Fucnion para cambiar la direccion I2C de un sensor
 // Parametro dirI2C corresponde a la direccion I2C del sensor
 // Parametro newdirI2C corresponde a la nueva direccion I2C que se le va a dar

{
   // A completar
}

unsigned int detectar_direccion (unsigned char *dirI2C)
 // Funcion para detectar la direccion I2C del sensor conectado
 // Si el sensor responde, la funcion devuelve 0 y actualiza el parametro dirI2C
 // Si no responde ningun sensor, devuelve 1
{
   // A completar
}

