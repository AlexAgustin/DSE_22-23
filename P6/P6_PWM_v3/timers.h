// Exporta una variable y funciones invocadas en otros modulos

#define LCD_LINE1 0
#define LCD_DATA1 1
#define LCD_LINE2 2
#define LCD_DATA2 3

#define PWM0_ACTIVE 0
#define PWM1_ACTIVE 1
#define PWM_RESTO 2

//Flag para inicializar el cronometro
extern int inicializar_crono;
extern unsigned int flag_PWM_T2;
extern unsigned int duty0;
extern unsigned int duty1;
extern unsigned int estado_PWM;
// declaracion de las funciones relacionadas con los temporizadores
// que se usan en otros modulos

void inic_Timer9(unsigned long ciclos);
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);
void inic_Timer7 ();
void inic_crono();
void cronometro();
void inic_Timer5 ();
void inic_Timer3 ();
void inic_Timer2_PWM ();

