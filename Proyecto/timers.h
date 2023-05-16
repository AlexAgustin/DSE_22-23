/* Funciones, constantes y variables a exportar a otros 
modulos para trabajar con los temporizadores
================================================

Autores: Alex y Amanda
Fecha: Mayo 2023
*/

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

//Relacionadas con T9
void inic_Timer9_delay(unsigned long ciclos);
void reinic_Timer9_CPU();
void restart_Timer9_CPU();
void stop_Timer9_CPU();
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);

//Relacionadas con T7/crono
void inic_Timer7_crono ();
void inic_crono();
void cronometro();

//Relacionadas con T5
void inic_Timer5_LCD ();

//Relacionadas con T3
void inic_Timer3_ADC ();

//Relacionadas con T2/senhales PWM/servos
void inic_Timer2_OCx ();
void inic_Timer4_movservos ();
void restart_Timer4_movservos();
void inic_Timer8_PWM();

//Relacionadas con T6/distancia
void inic_Timer6_dis();
void restart_Timer6_dis();

