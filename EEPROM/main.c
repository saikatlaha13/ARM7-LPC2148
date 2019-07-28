#include <lpc214x.h> 
#include "delay.h"
#include "lcd.h"
#include "i2c.h"


extern unsigned char RdData[50];
unsigned char WrData[16] ="Assisi-E Tech";

int main(void)
{ 

	PINSEL2=0x00000000;
	PINSEL1=0x00000000;
	PINSEL0=0x00000000; 
	IODIR0=0x70000000;
	IODIR1=0x00FF0000;
	LcdInit();
	LcdClr();
	I2cInit();
	Delay_ms(100);		

		I2cTransferByte(0xA0,0,16,WrData);
		Delay_ms(1000);										   
		I2cTransferByte(0xA0,0,0,'\0');	
		Delay_ms(100);	
		I2cTransferByte(0xA1,3,16,RdData);
		Delay_ms(1000);
		LcdString(RdData);
	while(1);
			
}


