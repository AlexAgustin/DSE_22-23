/* 
Contiene cuatro funciones auxiliares:
-  conversion_2dig: para la conversion de un valor de 2 digitos (digito -> caracteres).
-  conversion_3dig: para la conversion de un valor de 3 digitos (digito -> caracteres).
-  conversion_4dig: para la conversion de un valor de 4 digitos (digito -> caracteres).
-  gestion_cont: obtener y visualizar el porcentaje de uso de CPU actual y el maximo registrado.

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"

unsigned char tabla_carac[16]="0123456789"; // Tabla de caracteres

// Funcion para obtener los caracteres de un valor de dos digitos (<100)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion a partir de la cual queremos que la funcion devuelva los codigos ASCII
// En dir estara el caracter de mas peso y en dir+1 el de menos peso
void conversion_2dig (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>99) { //valor superior a lo contemplado (2 digitos)
        while(1); //espera infinita
    } else { //conversion
        dig=val/10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=val%10;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}
	

// Funcion para obtener los caracteres de un valor de tres digitos (<1000)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion a partir de la cual queremos que la funcion devuelva los codigos ASCII
// En dir estara el caracter de mas peso y en dir+2 el de menos peso
void conversion_3dig (unsigned char * dir, unsigned int val)
{
    unsigned int dig;
    int i,divisor = 100;
    if (val>999) { //valor superior a lo contemplado (3 digitos)
        while(1);  //espera infinita
    } else {  // conversion
        dig=val;
        for(i=0; i<2; i++){
            dig=dig/divisor;
            dig=tabla_carac[dig];
            *dir=dig;
            dir++;
            dig=val%divisor;
            divisor/=10; //actualizar divisor
        }
        dig=dig/divisor;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}

// Funcion para obtener los caracteres de un valor de cuatro digitos (<10000)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion a partir de la cual queremos que la funcion devuelva los codigos ASCII
// En dir estara el caracter de mas peso y en dir+3 el de menos peso
void conversion_4dig (unsigned char * dir, unsigned int val)
{
    unsigned int dig;
    int i,divisor = 1000;
    if (val>9999) { //valor superior a lo contemplado (4 digitos)
        while(1);  //espera infinita
    } else {  // conversion
        dig=val;
        for(i=0; i<3; i++){
            dig=dig/divisor;
            
            dig=tabla_carac[dig];
            *dir=dig;
            dir++;
            dig=val%divisor;
            divisor/=10; //actualizar divisor
        }
        dig=dig/divisor;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}

// Funcion para obtener y visualizar el porcentaje de uso de CPU actual
// y el porcentaje de uso de CPU maximo registrado
void gestion_cont(unsigned long cont)
{
    static unsigned long contmax = 0;
    unsigned int cont_aux=0;
    cont_aux = 100-((float)cont / (float)80001) * 100; // Obtener porcentaje de uso de CPU //80001 es el valor establecido como maximo (extaido del programa auxiliar))
    conversion_3dig(&Ventana_LCD[filacpu][poscpucur], cont_aux); //Guardar dicho porcentaje en VentanaLCD

    //Actualizar valor maximo de uso de CPU registrado si procede
    if (cont_aux>contmax){ //Si el nuevo valor es mayor que el maximo registrado...
        contmax = cont_aux; //Actualiza maximo
        conversion_3dig(&Ventana_LCD[filacpu][poscpumax], contmax);  //Visualizar nuevo valor maximo
    }
}