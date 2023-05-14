
// Variables y funciones exportadas para utilizarlas desde otros modulos

// VARIABLES
//=========================================================
extern unsigned int estado_PWM;
extern unsigned int OC_DUTY_MIN;
extern unsigned int OC_DUTY_MAX;
extern unsigned int flag_DUTY;
extern unsigned int flag_Duty_LCD;
extern unsigned int flag_posicion_segura;
extern unsigned int flag_calib;
extern unsigned int flag_estrella;
extern unsigned int flag_casa;
extern unsigned int flag_inic_pwm;
extern unsigned int duty[5];
extern unsigned int duty_min[5];
extern unsigned int duty_max[5];
extern unsigned int objetivopwm[7];

// FUNCIONES
//=========================================================
void visualizar_Duty();
void inic_PWM();
void inic_calib();
void posicion_segura();
void inic_OC1();
void inic_OC2();
void inic_OC3();
void inic_OC4();
void dibujar_estrella();
void dibujar_casa();
