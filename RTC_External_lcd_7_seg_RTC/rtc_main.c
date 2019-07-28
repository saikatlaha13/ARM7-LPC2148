#include <lpc214x.h> 
#include "delay.h"
#include "lcd.h"
#include "i2c.h"
#include<STRING.h>
#include<stdio.h>

#define DEV_WR_STAT  0x18	 // to confrm the ack fr device address
#define DATA_WR_STAT 0x28	  //  to confrm the ack fr data
//#define DEV_ADDR     0x4C	  //   device address
#define DEV_ADDR1    0x4C
#define	SetTimeDate  (IOPIN0 &(1<<0))

extern unsigned char GetTmDtVal[10];
unsigned char SetTmDtVal[7] ={0x00,0x23,0x02,0x06,0x10,0x01,0x14};
extern unsigned char RSEC,RMIN,RHOUR,RDOW,RDATE,RMONTH,RYEAR;
char *str1="I2c slave";
void fun1();
void fun2();
unsigned int flag=0;
char str[5];
char str2[5];

int main(void)
{
	#if 1
	I2cInit();
	LcdInit();
	LcdClr();	
	LcdGoToLine1();
	LcdString("Time: ");
	LcdGoToLine2();
	LcdString("Date: ");
	
	if(!(SetTimeDate))
	{
		I2cTransferByte(0xD0,0,8,SetTmDtVal);
	}
	#endif
					
	while(1)
	{
		//flag=0;
		Delay_ms(10);
		flag=0;
		fun1();
		flag=1;
		//Delay_ms(10);
		fun2();
		flag=0;
		//Delay_ms(10);
	}
}

void fun1()
{
	  I2cInit();		
	//	Delay_ms(10);													   
		Delay_ms(10);													   
		I2cTransferByte(0xD0,0,0,'\0');	
		Delay_ms(10);
		I2cTransferByte(0xD1,0,8,GetTmDtVal);
		ExtractData(GetTmDtVal);
		Delay_ms(10);
		LcdGoToXY(1,6);
		LcdDispRtcTime(RHOUR,RMIN,RSEC);
		LcdGoToXY(2,6);
		LcdDispRtcDate(RDATE,RMONTH,RYEAR);	 
	  i2c_stop();
	
	return;
}

void fun2()
{
	  i2c_init();
	  i2c_start();
	  i2c_write(DEV_ADDR1 ,DEV_WR_STAT);
		LCD_INIT();
	  //strcat(str,&RHOUR);
		//sprintf(result,"%d",i);
	  //Delay_ms(10);
	  sprintf(str,"%d",RMIN);
	  LCDSTR(0x80,"MIN=");
	  LCDSTR(0x85,str);
	  sprintf(str2,"%d",RDATE);
	  LCDSTR(0xC0,"DAY=");
	  LCDSTR(0xC5,str2);
	  //Delay_ms();
	  i2c_stop();
	  //flag=0;
	
	return;
}
