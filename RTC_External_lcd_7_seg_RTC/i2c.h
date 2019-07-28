#ifndef I2C_H
#define I2C_H

#define DATA_WR_STAT 0x28
#define DEV_WR_STAT  0x18	

void I2cInit(void);
void I2cISR(void)__irq;
void I2cTransferByte(unsigned int I2CAddr,unsigned char MemAddr,unsigned char Count,unsigned char *ap);
void ExtractData(unsigned char *ptr);
void i2c_stop(void);
void i2c_write(char dat, char status);
void i2c_start();
void LCD_CMD(unsigned char value);
void i2c_write(char dat, char status);
void LCD_DATA(unsigned char value);
void i2c_init(void);

#endif
