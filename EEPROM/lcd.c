#include <LPC214x.h>
#include "delay.h"
#include "lcd.h"


/**********************************************************/
/*					DEFINATION FOR LCD CONTROL PINS		  */
/*					RS=P0.28,RW=P0.29, EN=P0.30		   	  */
/*					LcdDataPort=P1.20-P1.23				  */
/**********************************************************/
#define RS (1<<28)
#define RW (1<<29)
#define EN (1<<30)
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xC0
#define BlankSpace ' '
#define LcdDataPort IOPIN1

/**********************************************************/
/*		LCD COMMAND FUNCTION						      */
/**********************************************************/

void LcdCmd(unsigned char Cmd)
{
	LcdDataPort=((Cmd & 0xF0)<<16);				//Send the commands to the LCD.
	IOCLR0=RS;						    //Set as Command or Instruction register to send the Command to the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);					//Controller to the LCD data pins.
	IOCLR0=EN;
		
	//Lcd_Delay_ms(10);				   //wait for few millisec

	LcdDataPort=((Cmd<<4) & 0x0F0)<<16;				//Send the commands to the LCD.
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
	LcdCmd(0x03);
	Lcd_Delay_ms(5);
	LcdCmd(0x02);
	Lcd_Delay_ms(5);
	LcdCmd(0x28);					   //Initialize the LCD by sending the Command 0x28
	Lcd_Delay_ms(5);
	LcdCmd(0x0C);					   //Dipslay ON and Cursor OFF
	Lcd_Delay_ms(5);
	LcdCmd(0x06);
	Lcd_Delay_ms(5);
	LcdCmd(0x01);					   //Clear The Display
	Lcd_Delay_ms(5);
}


/**********************************************************/
/*				LCD CLEAR FUNCTION						  */
/**********************************************************/

void LcdClr(void)						//LCD Clear Function
{
	LcdCmd(0x01);
	LcdCmd(LineOne);
}


/**********************************************************/
/*			LCD LINE1 FUNCTION							  */
/**********************************************************/

void LcdGoToLine1(void)				   //Fucntion to move the cursor to first line
{
	LcdCmd(LineOne);
}

/**********************************************************/
/*			LCD LINE2 FUNCTION							  */
/**********************************************************/

void LcdGoToLine2(void)				   	//Fucntion to move the cursor to first line
{
	LcdCmd(LineTwo);
}


/**********************************************************/
/*		LCD CHAR FUNCTION								  */
/**********************************************************/

void LcdData(unsigned char Chr)
{
	LcdDataPort=(Chr & 0xF0)<<16;				//Send the commands to the LCD.
	IOSET0=RS;							//Set as Data register to send the Data onto the LCD.
	IOCLR0=RW;							//Clear the Read/write=0.
	IOSET0=EN;							//Send High to Low Pulse to the LCD to Latch the Data From
	Lcd_Delay_ms(5);				    //Controller to the LCD data pins.
	IOCLR0=EN;

	//Lcd_Delay_ms(10);				   //wait for few millisec

	LcdDataPort=((Chr<<4) & 0x0F0)<<16;			//Send the commands to the LCD.
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
		LcdData(*Str++);
	}
}

/**********************************************************/
/*			LCD GOTO XY FUNCTION						  */
/**********************************************************/

void LcdGoToXY(unsigned char row,unsigned char col)
{
    switch(row)	
	{
        case 1:
            // position for line 1
            row=0x80 ;
            break;
            
        case 2:
            // position for line 2
            row=0xC0 ;
            break;          
        default:
            break;           
    }
    LcdCmd(row-1+col);	
}
/**********************************************************/
/*			LCD Display Number FUNCTION					  */
/**********************************************************/
void LcdDisplayNumber(unsigned int num)				// Function to Display the Numbers onto the LCD
{
	//LcdData((num/10000)+0x30);
	//num=num%10000;
	//LcdData((num/1000)+0x30);
	//num=num%1000;
	//LcdData((num/100)+0x30);
	//num=num%100;
	LcdData((num/10)+0x30);
	LcdData((num%10)+0x30);
}


/**********************************************************/
/*			LCD Scroll Msg FUNCTION					      */
/**********************************************************/

void LcdScrollMsg(unsigned char *msg_ptr)			// Function to Scroll th display onto the LCD
{
	unsigned char i,j;
	LcdCmd(0x0c);
	for(i=0;msg_ptr[i];i++)
	{
		LcdGoToLine1();
		for(j=0;j<LCDMaxChars && msg_ptr[i+j];j++)
		{
			LcdData(msg_ptr[i+j]);
		}
		for(j=j;j<LCDMaxChars;j++)
		{
			LcdData(BlankSpace);
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
		LcdDisplayNumber(hour);
		LcdData(':');
		LcdDisplayNumber(min);
		LcdData(':');
		LcdDisplayNumber(sec);
}

/**********************************************************/
/*			LCD Display Date FUNCTION					  */
/**********************************************************/

void LcdDispRtcDate(unsigned day, unsigned char month, unsigned char year)
// Function to Display the RTC Date
{
		LcdDisplayNumber(day);
		LcdData('/');
		LcdDisplayNumber(month);
		LcdData('/');
		LcdDisplayNumber(year);
}









