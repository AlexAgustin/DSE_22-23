/* Funciones y variables a exportar a otros 
modulos para trabajar con el modulo ADC1
================================================

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

// Funciones que se llaman desde otros modulos
void inic_ADC1 ();
void tratar_valorADC1 ();

// variables usadas en otros modulos
extern unsigned int flag_ADC;
extern unsigned long num_conversiones;