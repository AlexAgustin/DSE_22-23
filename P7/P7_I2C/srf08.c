/*
Funciones para utilizar el sensor de distancia

Autoras/es: Alex y Amanda

Fecha: Marzo 2023
*/


#include "i2c_funciones.h"

unsigned int inic_medicion_dis (unsigned char dirI2C)
 // Puesta en marcha de una medicion
 // Parametro dirI2C corresponde a la direccion I2C del sensor
{
   LDByteWriteI2C_1(dirI2C,0,0x51); //Mandar al registro 0 (comandos) del esclavo (dirI2C)
                                    //el comando para que de los resultados en cm (0x51).
}

unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) 
 // Funcion para leer la distancia medida
 // Parametro dirI2C corresponde a la direccion I2C del sensor
 // Parametro dis proporciona la direccion de un array en el que devolver la medicion
 // Cada medicion 2 bytes
{
    unsigned int dist_media;
    LDByteReadI2C_1(dirI2C,2,dis,2); //Leer del registro 2 (distancia) del esclavo (dirI2C)
                                    //los dos bytes corresponientes a la distancia media.
    dist_media = dis&0xFF00 + dis&0x00FF;
    
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

