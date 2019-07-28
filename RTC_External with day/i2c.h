#ifndef I2C_H
#define I2C_H

void I2cInit(void);
void I2cISR(void)__irq;
void I2cTransferByte(unsigned int I2CAddr,unsigned char MemAddr,unsigned char Count,unsigned char *ap);
void ExtractData(unsigned char *ptr);

#endif
