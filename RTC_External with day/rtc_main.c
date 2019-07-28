#include <lpc214x.h> 
#include "delay.h"
#include "lcd.h"
#include "i2c.h"

#define	 SetTimeDate (IOPIN0 &(1<<0))
extern unsigned char GetTmDtVal[10];
unsigned char SetTmDtVal[7] ={0x00,0x40,0x06,0x04,0x17,0x01,0x14};
extern unsigned char RSEC,RMIN,RHOUR,RDOW,RDATE,RMONTH,RYEAR;	
unsigned char *DOFW[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

int main(void)
{ 
	PINSEL2=0x00000000;
	IODIR0=0x70000000;
	IODIR1=0x00FF0000;
	LcdInit();
	LcdClr();
	I2cInit();
	LcdGoToLine1();
	LcdString("Tm:");
	LcdGoToLine2();
	LcdString("Dt:");
	
	if(!(SetTimeDate))
	{
		I2cTransferByte(0xD0,0,8,SetTmDtVal);
	}
				
					
	while(1)
	{ 	
		Delay_ms(10);													   
		I2cTransferByte(0xD0,0,0,'\0');	
		Delay_ms(10);
		I2cTransferByte(0xD1,0,8,GetTmDtVal);
		ExtractData(GetTmDtVal);
		Delay_ms(10);
		LcdGoToXY(1,4);
		LcdDispRtcTime(RHOUR,RMIN,RSEC);
		LcdGoToXY(1,13);
		LcdString(DOFW[RDOW]);
		LcdGoToXY(2,4);
		LcdDispRtcDate(RDATE,RMONTH,RYEAR);	 		
	}
}
