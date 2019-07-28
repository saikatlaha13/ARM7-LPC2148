#include <lpc214x.h>

#include "delay.h"
 
 
 // SEMENTS a to g --  p1.16 to p1.23
 // control seg    -- p0.16 to p0.19
 // ADC i.e port one end vcc , other end gnd , center p0.28
 
 
void Delay_Ms(unsigned int count);
void halfsecondsdelay(int a);
 
   


#define dsp1 (1<<16)
#define dsp2 (1<<17)
#define dsp3 (1<<18)
#define dsp4 (1<<19)
void lightNumber(int numberToDisplay);
void dsp(int number);


void ADC_Init(void);
unsigned int ADC_Read(unsigned char channel);
void ADC(void);
unsigned int value=0;


int main()
{
	 	
  IODIR0=0x000F0000;			     
	IODIR1=0x00FF0000;
	
	IOCLR1=0x00FF0000;
	IOSET1= 0x3F<<16;  
	Delay_ms(100);
  ADC_Init();
	
	
while(1)
	{
	  
	ADC();
  dsp(value);		
				
	}
}


void ADC_Init(void)
{
 
	PINSEL1 |=0x15000000;//0X0A400000;	//configuring AD0.4
	PCONP |= 0x00001000; //Power on the A/D converter 0
  AD0CR=0x00200400;	// PDN=1(operational mode), Operate at 4MHz. 
}


void ADC(void)
{	 
	unsigned long adc_data;
    unsigned char ones,tens,hundreds,thousands;
	  unsigned long temp;
 
    adc_data = ADC_Read(1);// ch =1
	
		value = adc_data;
			 
		
}


unsigned int ADC_Read(unsigned char channel)
{
static unsigned val;
   AD0CR &= ~(0x070000FF);	
	 AD0CR |= ((unsigned long)(1)<<channel);	 	  //Select the A/D channel
	 AD0CR |= (1 << 24);	//start convertion			 	  				  
   while(!(AD0GDR & (0x80000000)));  	 //Wait for the conversion to complete
	AD0CR &= ~(0x070000FF); 
	val = AD0GDR;
	 val = ((val>>6 & 0x03FF));					 //Extract A/D result
	 return(val); 
}


void dsp(int number)
	{

	int temp;
	int ones;
	int tens;
	int hundreds;
	int thousands;

	temp=number;

	ones = temp % 10;
	temp = temp / 10;
	tens = temp % 10;
	temp = temp / 10;
	hundreds = temp % 10;
	temp = temp / 10;
	thousands=temp %10;
		
	IOSET0= dsp1;
	IOCLR1=0X00FF0000;
	lightNumber(ones);
	
	Delay_ms(10);
	IOCLR0= dsp1;
	 
IOSET0= dsp2;
IOCLR1=0X00FF0000;
lightNumber(tens);
//IOSET1=hundreds<<16; 
Delay_ms(5);
IOCLR0= dsp2;



IOSET0= dsp3;
IOCLR1=0X00FF0000;
lightNumber(hundreds);

Delay_ms(1);
IOCLR0= dsp3;

IOSET0= dsp4;
IOCLR1=0X00FF0000;
lightNumber(thousands);
 
Delay_ms(1);
IOCLR0= dsp4;
	}

	 
	
	
	void lightNumber(int numberToDisplay)
  		{

  		switch (numberToDisplay)
  		{
  		case 0:IOSET1= 0x3F<<16; //0
  		break;
  		case 1:IOSET1= 0x06<<16;//1
  		break;
  		case 2:IOSET1 = 0x5B<<16; //2
  		break;
  		case 3:IOSET1 = 0x4F<<16; //3
  		break;
  		case 4:IOSET1 = 0x66<<16; //4
  		break;
  		case 5:IOSET1 = 0x6D<<16; //5
  		break;
  		case 6:IOSET1 = 0x7D<<16; //6
  		break;
  		case 7:IOSET1= 0x07<<16; //7
  		break;
  		case 8:IOSET1 = 0x7F<<16; //8
  		break;
  		case 9:IOSET1= 0x6F<<16; //9
  		break;
  		}
  		}
			
			
			
			void Delay_Ms(unsigned int count)
{
    volatile unsigned int j,k;
    for (j=0;j<count;j++)
	{
        for (k=0;k<6000;k++)
		{
        
    }
  }
}




