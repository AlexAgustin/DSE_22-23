// Exporta constantes, variables y funciones relacionadas con los timers
// invocadas en otros modulos

// CONSTANTES
//=========================================================
#define PWM0_ACTIVE 0
#define PWM1_ACTIVE 1
#define PWM2_ACTIVE 2
#define PWM3_ACTIVE 3
#define PWM4_ACTIVE 4
#define PWM_RESTO 5


// VARIABLES
//=========================================================
extern int inicializar_crono; //Flag para inicializar el cronometro
extern unsigned int reached; //Cantidad de servos que han alcanzado su posicion debida


// FUNCIONES
// declaracion de las funciones relacionadas con los temporizadores
// que se usan en otros modulos
//=========================================================
void inic_Timer9_delay(unsigned long ciclos);
void reinic_Timer9_CPU();
void restart_Timer9_CPU();
void stop_Timer9_CPU();
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);

void inic_Timer7_crono ();
void inic_crono();
void cronometro();

void inic_Timer5_LCD ();

void inic_Timer3_ADC ();

void inic_Timer2_OCx ();
void inic_Timer4_movservos ();
void restart_Timer4_movservos();
void inic_Timer8_PWM();

void inic_Timer6_dis();
void restart_Timer6_dis();

