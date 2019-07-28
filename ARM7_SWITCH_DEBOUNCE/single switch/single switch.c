#include <lpc214x.h>

void tiny_delay(void);
#define sw (1<<16)
#define led (1<<17)

int main(void)
{
	int flag=0, pinSamplePrevious=0, pinSampleCurrent=0;
	IO1DIR &= ~sw; 
	IO1DIR |= led;
	
	pinSamplePrevious = IO1PIN & sw; //Initial Sample
	while(1)
	{
		pinSampleCurrent = IO1PIN & sw; //New Sample
		
		if( pinSampleCurrent != pinSamplePrevious )
		{
			
			tiny_delay(); // momentary delay	
			pinSampleCurrent = IO1PIN & sw;
			
			if( pinSampleCurrent != pinSamplePrevious )
			{
				if(flag) 
				{
					IO1SET |= led;
					flag=0; 
				} 
				else
				{
					IO1CLR |= led; // drive P0.30 Low
					flag=1; //next time 'if' part will excute
				}
				pinSamplePrevious = pinSampleCurrent; 
			}
		}
	}
	return 0; // this wont execute ever :P
}

void tiny_delay(void)
{
	int z,c;
	c=0;
	for(z=0; z<1500; z++) //Higher value for higher clock speed 
	{
		z++;
	}
}