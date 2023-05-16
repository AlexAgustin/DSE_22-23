/* 
Funciones y variables relacionadas con el sensor de distancia
a exportar a otros ficheros.
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