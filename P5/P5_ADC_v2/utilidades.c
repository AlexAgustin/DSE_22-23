/* 
Contiene dos funciones auxiliares:
 * conversion_tiempo: para la conversion del tiempo (digito -> caracteres).
 * conversion_ADC: para la conversion de un valor de 4 digitos (digito -> caracteres).

Autores: Alex Agustin y Amanda Sin
Fecha: Febrero 2023
*/

#include "p24HJ256GP610A.h"

// Funcion para obtener los caracteres de un valor de dos digitos (<100)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion a partir de la cual queremos que la funcion devuelva los codigos ASCII
// En dir estara el caracter de mas peso y en dir+1 el de menos peso


unsigned char tabla_carac[16]="0123456789";
void conversion_tiempo (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>99) {
        while(1);    
    } else {
        dig=val/10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=val%10;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}
	
// Funcion para obtener los caracteres de un valor de cuatro digitos (<9999)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion a partir de la cual queremos que la funcion devuelva los codigos ASCII
// En dir estara el caracter de mas peso y en dir+3 el de menos peso
void conversion_ADC (unsigned char * dir, unsigned int val)
{
    unsigned int dig;
    int i,divisor = 1000;
    if (val>9999) {
        while(1);    
    } else { 
        dig=val;
        for(i=0; i<3; i++){
            dig=dig/divisor;
            
            dig=tabla_carac[dig];
            *dir=dig;
            dir++;
            dig=val%divisor;
            divisor/=10;
        }
        dig=dig/divisor;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}