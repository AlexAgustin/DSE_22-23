//Exporta constantes, variables y funciones utilizadas externamente
// constantes y variables
extern const unsigned char Mens_LCD_1[16],Mens_LCD_2[16],Mens_LCD_3[16];
extern const unsigned char Mens_LCD_4[16],Mens_LCD_5[16],Mens_LCD_6[16];
extern unsigned char Ventana_LCD[2][16];

// declaracion de las funciones relacionadas con memoria
// que se usan en otros modulos
void copiar_FLASH_RAM (const unsigned char *texto, unsigned int i);

