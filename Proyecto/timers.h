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
extern unsigned int flag_All_Reached;

// FUNCIONES
// declaracion de las funciones relacionadas con los temporizadores
// que se usan en otros modulos
//=========================================================
void inic_Timer9_delay(unsigned long ciclos);
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);
void inic_Timer7_crono ();
void inic_crono();
void cronometro();
void inic_Timer5_LCD ();
void inic_Timer3_ADC ();
void inic_Timer2_OCx ();
void inic_Timer4_movservos ();
void inic_Timer8_PWM();
void inic_Timer6_dis();
void restart_Timer6();

