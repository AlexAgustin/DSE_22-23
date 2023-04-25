
// declaracion de las funciones relacionadas con LCD
// que se usan en otros modulos

void lcd_cmd(char cmd);	
void lcd_data(char data);
void Init_LCD();
void puts_lcd (unsigned char *data, unsigned char count);
void line_1();
void line_2();

//variables que se usaran en otros ficheros
extern unsigned int fila1, fila2;

