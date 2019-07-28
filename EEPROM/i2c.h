#ifndef I2C_H
#define I2C_H

#define	NUL			  '\0' 
#define DELAY_COUNT   8555 
 
#define		AA   2 
#define		SI   3 
#define		STO  4 
#define		STA  5 
#define		I2EN 6 
 
#define		START		0x08	 
#define		R_START		0x10 
#define		MT_SLA_ACK 	0x18 
#define		MT_DATA_ACK	0x28 
 
#define		MT_RX_SLA_ACK 	0x40 
#define		MT_RX_DATA_ACK	0x50 
#define		MT_RX_DATA_NACK	0x58 
 
#define 	SLA_W		0xA0 
#define 	SLA_R		0xA1 

void I2cInit(void);
void I2cISR(void)__irq;
void I2cTransferByte(unsigned int I2CAddr,unsigned char MemAddr,unsigned char Count,unsigned char *ap);
void delay(int count);
void putStrE(unsigned char *str,unsigned short Addr);

#endif
