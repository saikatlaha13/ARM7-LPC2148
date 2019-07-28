#include <lpc214x.h>

void tiny_delay(void);
#define sw (1<<16)
#define led (1<<17)

int main(void)
{
	int flag=0, pinSamplePrevious, pinSampleCurrent;
	IO1DIR &= ~sw; 
	IO1DIR |= led;
	
	pinSamplePrevious = IO0PIN & sw; //Initial Sample

	while(1)
	{
		pinSampleCurrent = IO0PIN & sw; //New Sample
		
		if( pinSampleCurrent != pinSamplePrevious )
		{
			//P0.7 might get low or high momentarily due to noise depending the external conditions or some other reason
			//hence we again take a sample to insure its not due to noise
			
			tiny_delay(); // momentary delay
			
			// now we again read current status of P0.7 from IO0PIN	
			pinSampleCurrent = IO0PIN & sw;
			
			if( pinSampleCurrent != pinSamplePrevious )
			{
				//State of P0.7 has indeed changed
				if(flag) //First time Flag will be = 0 hence else part will execute
				{
					IO0SET |= led; // drive P0.30 High
					flag=0; //next time 'else' part will excute
				} 
				else
				{
					IO0CLR |= led; // drive P0.30 Low
					flag=1; //next time 'if' part will excute
				}
				
				//set current value as previous since it has been processed
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
		c++;
	}
}