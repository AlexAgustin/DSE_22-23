// Exporta variables y funciones invocadas en otros modulos

// CONSTANTES
//=========================================================
#define LCD_LINE1 0
#define LCD_DATA1 1
#define LCD_LINE2 2
#define LCD_DATA2 3

#define PWM0_ACTIVE 0
#define PWM1_ACTIVE 1
#define PWM2_ACTIVE 2
#define PWM3_ACTIVE 3
#define PWM4_ACTIVE 4
#define PWM_RESTO 5


// VARIABLES
//=========================================================
extern int inicializar_crono; //Flag para inicializar el cronometro

// FUNCIONES
// declaracion de las funciones relacionadas con los temporizadores
// que se usan en otros modulos
//=========================================================
void inic_Timer9(unsigned long ciclos);
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);
void inic_Timer7 ();
void inic_crono();
void cronometro();
void inic_Timer5 ();
void inic_Timer3 ();
void inic_Timer2_OC1 ();
void inic_Timer4_OC3 ();
void inic_Timer8_PWM();
void inic_Timer6();
void restart_Timer6();

