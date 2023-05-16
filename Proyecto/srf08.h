/* Funciones y variables a exportar a otros 
modulos relacionadas con el sensor de distancia
================================================

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

// FUNCIONES A EXPORTAR 
//=========================================================
void gestion_dis(unsigned char dirI2C);
unsigned int inic_medicion_dis (unsigned char dirI2C);
unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) ;
void cambiar_direccion (unsigned char dirI2C, unsigned char newdirI2C);
unsigned int detectar_direccion (unsigned char *dirI2C);


// VARIABLES A EXPORTAR 
//=========================================================
extern unsigned int flag_dis;
extern unsigned int dis_media;