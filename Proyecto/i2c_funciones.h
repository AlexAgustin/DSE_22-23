// Funciones relacionadas con I2C utilizadas en otros modulos

void InitI2C_1();
unsigned int LDByteReadI2C_1(unsigned char I2C_address, unsigned char Register_address, unsigned char *Data, unsigned char Length);
unsigned int LDByteWriteI2C_1(unsigned char I2C_address, unsigned char Register_address, unsigned char data);
