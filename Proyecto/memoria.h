/* Funcion y variable a exportar a otros 
modulos para trabajar con la memoria
================================================

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

// VARIABLE
//=========================================================
extern unsigned char Ventana_LCD[NFILAS][NCOLUMNAS];    //Variable que contiene toda la informacion de control a visualizar en pantalla (LCD/UART)

// FUNCION
//=========================================================
void actualizar_Ventana_LCD();  //Funcion que actualiza la visualizacion de informacion en LCD una vez el programa se da por comenzado
