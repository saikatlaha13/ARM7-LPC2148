#include <LPC214x.h>
#include "delay.h"
#include "lcd.h"


/**********************************************************/
/*					DEFINATION FOR LCD CONTROL PINS		  */
/*					RS=P0.4,RW=P0.5, EN=P0.6		   	  */
/*					LcdDataPort=P1.20-P1.23	to D4...D7	  */
/**********************************************************/
#define RS (1<<19)
#define RW (1<<20)
#define EN (1<<21)
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xC0
#define BlankSpace ' '
#define LcdDataPort IOPIN0

/**********************************************************/
/*		LCD COMMAND FUNCTION						      */
/**********************************************************/

void LcdCmd4Bit(unsigned char Cmd)
{
	IOPIN0=((Cmd & 0xF0)<<0);				//Send the commands to the LCD.//LcdDataPort
	IOCLR0=RS;						    //Set as Command or Instruction register to send the Command to the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);					//Controller to the LCD data pins.
	IOCLR0=EN;
		
	//Lcd_Delay_ms(10);				   //wait for few millisec

	LcdDataPort=((Cmd<<4) & 0x0F0)<<0;				//Send the commands to the LCD.
	IOCLR0=RS;						    //Set as Command or Instruction register to send the Command to the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);					//Controller to the LCD data pins.
	IOCLR0=EN;	
}


/**********************************************************/
/*			LCD INIT FUNCTION							  */
/**********************************************************/

void LcdInit(void)
{
	LcdCmd4Bit(0x03);
	Lcd_Delay_ms(5);
	LcdCmd4Bit(0x02);
	Lcd_Delay_ms(5);
	LcdCmd4Bit(0x28);					   //Initialize the LCD by sending the Command 0x28
	Lcd_Delay_ms(5);
	LcdCmd4Bit(0x0C);					   //Dipslay ON and Cursor OFF
	Lcd_Delay_ms(5);
	LcdCmd4Bit(0x06);
	Lcd_Delay_ms(5);
	LcdCmd4Bit(0x01);					   //Clear The Display
	Lcd_Delay_ms(5);
}


/**********************************************************/
/*				LCD CLEAR FUNCTION						  */
/**********************************************************/

void LcdClr(void)						//LCD Clear Function
{
	LcdCmd4Bit(0x01);
	LcdCmd4Bit(LineOne);
}


/**********************************************************/
/*			LCD LINE1 FUNCTION							  */
/**********************************************************/

void LcdGoToLine1(void)				   //Fucntion to move the cursor to first line
{
	LcdCmd4Bit(LineOne);
}

/**********************************************************/
/*			LCD LINE2 FUNCTION							  */
/**********************************************************/

void LcdGoToLine2(void)				   	//Fucntion to move the cursor to first line
{
	LcdCmd4Bit(LineTwo);
}


/**********************************************************/
/*		LCD CHAR FUNCTION								  */
/**********************************************************/

void LcdChar4Bit(unsigned char Chr)
{
	LcdDataPort=(Chr & 0xF0)<<0;				//Send the commands to the LCD.
	IOSET0=RS;							//Set as Data register to send the Data onto the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);				    //Controller to the LCD data pins.
	IOCLR0=EN;

	//Lcd_Delay_ms(10);				   //wait for few millisec

	LcdDataPort=((Chr<<4) & 0x0F0)<<0;			//Send the commands to the LCD.
	IOSET0=RS;							//Set as Data register to send the Data onto the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);				    //Controller to the LCD data pins.
	IOCLR0=EN;	
}

/**********************************************************/
/*			LCD STRING FUNCTION							  */
/**********************************************************/
void LcdString(unsigned char *Str)		// Function to Display the String
{
	while(*Str!='\0')
	{
		LcdChar4Bit(*Str++);
	}
}

/**********************************************************/
/*			LCD GOTO XY FUNCTION						  */
/**********************************************************/
void LcdGoToXY(unsigned char row,unsigned char col)	  // Function to set the cursor postion
{													  // according to user define by filling row and column
	char pos;
	if(row<LCDMaxLines)
	{
		pos=LineOne|(row<<6);
		if(col<LCDMaxChars)
		{
			pos=pos+col;
		}
		LcdCmd4Bit(pos);
	}
}

/**********************************************************/
/*			LCD Display Number FUNCTION					  */
/**********************************************************/
void LcdDisplayNumber(unsigned int num)				// Function to Display the Numbers onto the LCD
{
	LcdChar4Bit((num/10000)+0x30);
	num=num%10000;
	LcdChar4Bit((num/1000)+0x30);
	num=num%1000;
	LcdChar4Bit((num/100)+0x30);
	num=num%100;
	LcdChar4Bit((num/10)+0x30);
	LcdChar4Bit((num%10)+0x30);
}


/**********************************************************/
/*			LCD Scroll Msg FUNCTION					      */
/**********************************************************/

void LcdScrollMsg(unsigned char *msg_ptr)			// Function to Scroll th display onto the LCD
{
	unsigned char i,j;
	LcdCmd4Bit(0x0c);
	for(i=0;msg_ptr[i];i++)
	{
		LcdGoToLine1();
		for(j=0;j<LCDMaxChars && msg_ptr[i+j];j++)
		{
			LcdChar4Bit(msg_ptr[i+j]);
		}
		for(j=j;j<LCDMaxChars;j++)
		{
			LcdChar4Bit(BlankSpace);
		}
		Lcd_Delay_ms(300);
	}
}

/**********************************************************/
/*			LCD Display RTC FUNCTION					  */
/**********************************************************/

void LcdDispRtcTime(unsigned hour, unsigned char min, unsigned char sec)
												// Function to Display the RTC time
{
	LcdChar4Bit(((hour>>4)&0x0F)+0x30);
	LcdChar4Bit((hour&0x0F)+0x30);
	LcdChar4Bit(':');

	LcdChar4Bit(((min>>4)&0x0F)+0x30);
	LcdChar4Bit((min&0x0F)+0x30);
	LcdChar4Bit(':');

	LcdChar4Bit(((sec>>4)&0x0F)+0x30);
	LcdChar4Bit((sec&0x0F)+0x30);
}

/**********************************************************/
/*			LCD Display Date FUNCTION					  */
/**********************************************************/

void LcdDispRtcDate(unsigned day, unsigned char month, unsigned char year)
												// Function to Display the RTC Date
{
	LcdChar4Bit(((day>>4)&0x0F)+0x30);
	LcdChar4Bit((day&0x0F)+0x30);
	LcdChar4Bit('/');

	LcdChar4Bit(((month>>4)&0x0F)+0x30);
	LcdChar4Bit((month&0x0F)+0x30);
	LcdChar4Bit('/');

	LcdChar4Bit(((year>>4)&0x0F)+0x30);
	LcdChar4Bit((year&0x0F)+0x30);
}









