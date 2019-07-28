#include <lpc214x.h>
#define sw (1<<16)
#define led (1<<17)

// default state is 0 (pulled down external)
 // P1.16 is set as input
 // P1.17 is set as output 

int main(void)
{
		
	PINSEL1 = 0x00000000;
  IO1DIR &=  ~sw;           // P1.16 is set as input
	IO1DIR =  led;
    
    while(1)
		{
			if(!(IO1PIN & sw))    // true when switch reads zero
			{
				IO1SET = led;
			}
			else
			{
				IO1CLR= led;
      }
		}				
}

