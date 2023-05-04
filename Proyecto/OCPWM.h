
// Variables y funciones exportadas para utilizarlas desde otros modulos

// VARIABLES
//=========================================================
extern unsigned int DUTY_MIN;
extern unsigned int DUTY_MAX;
extern unsigned int OC_DUTY_MIN;
extern unsigned int OC_DUTY_MAX;
extern unsigned int flag_DUTY;
extern unsigned int flag_Duty_LCD;
extern unsigned int duty[5];
extern unsigned int estado_PWM;

// FUNCIONES
//=========================================================
void visualizar_Duty();
void inic_PWM();
void posicion_segura();
