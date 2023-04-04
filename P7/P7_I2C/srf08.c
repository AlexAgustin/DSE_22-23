/*
Funciones para utilizar el sensor de distancia.

Contiene las funciones para gesetionar la medicion de la distancia, 
iniciar la medicion, leer la medicion, 
cambiar la direccion y detectar la direccion.

Autoras/es: Alex y Amanda

Fecha: Marzo 2023
*/

#include "p24HJ256GP610A.h"
#include "i2c_funciones.h"
#include "timers.h"
#include "commons.h"
#include "memoria.h"
#include "utilidades.h"
#include "srf08.h"

unsigned int flag_dis = 0;

//Gestionar la medicion de la distancia
void gestion_dis(unsigned char dirI2C)
{
    
    unsigned char dis[2];
    unsigned int dis_media;

    if (leer_medicion(dirI2C,dis)) { // Lectura de la distancia media
       LATAbits.LATA1=1; // Encender led D4
       while(1); //Espera infinita
    }
    dis_media = (dis[0]<<8) | dis[1]; //Obtener la distancia media a partir de las dos posiciones del array dis

    conversion_4dig(&Ventana_LCD[0][posdis],dis_media); //Guardar distancia media en Ventana_LCD para su proyeccion en la pantalla
   
    if (inic_medicion_dis(dirI2C)){ // Puesta en marcha de una nueva medicion
       LATAbits.LATA3=1; // Encender led D6
       while(1); //Espera infinita
    }
    restart_Timer6(); // Puesta en marcha del temporizador T6
    flag_dis = 0; // puesta a 0 del flag para medir la distancia
}

unsigned int inic_medicion_dis (unsigned char dirI2C)
 // Puesta en marcha de una medicion
 // Parametro dirI2C corresponde a la direccion I2C del sensor
{ //Mandar al registro 0 (comandos) del esclavo (dirI2C)
                                    //el comando para que de los resultados en cm (0x51).
    return (LDByteWriteI2C_1(dirI2C,REG_COM,0x51));
}

unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) 
 // Funcion para leer la distancia media
 // Parametro dirI2C corresponde a la direccion I2C del sensor
 // Parametro dis proporciona la direccion de un array en el que devolver la medicion
 // Cada medicion 2 bytes
{
   return(LDByteReadI2C_1(dirI2C,REG_DATA,dis,2)); //Leer del registro 2 (distancia) del esclavo (dirI2C)
                                    //los dos bytes corresponientes a la distancia media.
}


void cambiar_direccion (unsigned char dirI2C, unsigned char newdirI2C)
 // Funcion para cambiar la direccion I2C de un sensor
 // Parametro dirI2C corresponde a la direccion I2C del sensor
 // Parametro newdirI2C corresponde a la nueva direccion I2C que se le va a dar
{
    if (LDByteWriteI2C_1(dirI2C,REG_COM,0xA0)==0)
        if (LDByteWriteI2C_1(dirI2C,REG_COM,0xAA)==0)
            if (LDByteWriteI2C_1(dirI2C,REG_COM,0xA5)==0)
                if (LDByteWriteI2C_1(dirI2C,REG_COM,newdirI2C)==0)
                    return;
    LATAbits.LATA7=1; // Encender led D10
    while(1); //Espera infinita
    
}

unsigned int detectar_direccion (unsigned char *dirI2C)
 // Funcion para detectar la direccion I2C del sensor conectado
 // Si el sensor responde, la funcion devuelve 0 y actualiza el parametro dirI2C
 // Si no responde ningun sensor, devuelve 1
{
    unsigned char dis;
    unsigned char i;
    for (i=0xE0; i<=0xFE;i+=2){
        LDByteReadI2C_1(i,REG_COM,&dis,1);
        if (dis!=0xFF && dis!=0x00) {
            *dirI2C = i;
            return (0);
        }
    }
    return(1);
}

