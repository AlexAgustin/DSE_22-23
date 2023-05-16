/* Funciones y variables a exportar a otros 
modulos para trabajar con el modulo LCD
================================================

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

// FUNCIONES
// declaracion de las funciones relacionadas con LCD
// que se usan en otros modulos
//=========================================================
void Init_LCD();            //Inicializacion de la pantalla LCD
void lcd_data (char data);  //Funcion para mandar un dato
void lcd_cmd (char cmd);    //Funcion para mandar un comando

// VARIABLES
// variables que se usan en otros ficheros
//=========================================================
extern unsigned int fila1, fila2; //filas de Ventana_LCD a mostrar en la LCD

