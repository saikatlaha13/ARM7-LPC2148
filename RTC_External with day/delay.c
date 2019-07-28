#include "delay.h"

/**********************************************************/
/*			LCD DELAY FUNCTION  						  */
/**********************************************************/

void Delay_ms(unsigned int dlyms)
{
	int k,l;
	for(l=0;l<dlyms;l++)
	for(k=0;k<7750;k++);
}

void Delay_sec(unsigned int dlysec)
{
	dlysec=dlysec*1000;
	Delay_ms(dlysec);
}

void Lcd_Delay_ms(unsigned int lcddly)
{
	Delay_ms(lcddly);
}
