#include <lpc214x.h>
#include "delay.h"
#include "lcd.h"

int main()
{
	/**************************************************************************/
	/* In LPC2129; LCD is connected to Pin no, Data Port P1.20-P1.23		  */
	/* 					RS=P0.10,RW=P0.11,EN=P0.12							  */
	/**************************************************************************/
	PINSEL1=0x00;
	PINSEL2=0x00;
	IODIR1=0xF0000000;
	IODIR0=0xffffffff;
	LcdInit();
	LcdClr();										// Clear the LCD
//	LcdScrollMsg("Assisi-E Tech");				    // Scroll the msg from right to left present in the string
	while(1)
	{
		LcdGoToXY(0,0);						  			// Cursor position is set to first position
		LcdString("***QVEMANA IT****");					    // Display string content
		LcdGoToXY(1,0);									// Move The cursor position to the 2 line 3 position
		LcdString("LOAD CELL BASED ");						// Dipslay the string value
		LcdCmd4Bit(0X94);								// Move The cursor position to the 2 line 3 position
		LcdString("FUEL MEASUREMENT");
		Delay_sec(5);								    // wait for 5 sec 
		
	}
}
