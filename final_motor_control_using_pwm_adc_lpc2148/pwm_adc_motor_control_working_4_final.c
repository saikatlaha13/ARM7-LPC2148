// pin 0.1 is pwm3 output pin
// adc input pin is 0.28

#include <lpc214x.h>
#include <stdint.h>
#include <stdbool.h>

//volatile bool driver_ip1 = 1;
//volatile bool driver_ip2 = 0;
volatile uint32_t result = 20;

void delay_ms(uint16_t j)
{
  uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);                               /* loop to generate 1 milisecond delay with Cclk = 60MHz */
	}
}

// interrupt request for PWM3
__irq void PWM_ISR (void)
{
	if ( PWMIR & 0x0001 )
	{
		PWMIR = 0x0001;	                                     /* Clear interrupt for PWMMR0 */
	}
	
	if ( PWMIR & 0x0008 )
	{
		PWMIR = 0x0008;                                      /* Clear interrupt for PWMMR3 */
	}	
	VICVectAddr = 0x00000000;
}

// interrupt handler for ADC0
__irq void ADC0_ISR (void)
{
	result = AD0DR1;	
	result = (result>>6);
	result = (result & 0x000003FF);
	VICVectAddr = 0x00000000;
}

int main (void)
{	
	float percent;
	PINSEL0 = PINSEL0 | 0x000000C0;                        /* Configure P0.3 as EINT1 */
	
	PINSEL0 = PINSEL0 | 0x00000008;                        /* Configure P0.1 as PWM3 output */
	PINSEL1 = 0x01000000;                                  /* P0.28 as AD0.1 input ADC0 channel 1 */	

	// PWM interrupt service routine configuration
	VICVectAddr0 = (unsigned) PWM_ISR;                     /* T0 ISR Address */
	VICVectCntl0 = (0x00000020 | 8);                       /* Enable PWM IRQ slot */
	VICIntEnable = VICIntEnable | 0x00000100;              /* Enable PWM interrupt */
	VICIntSelect = VICIntSelect | 0x00000000;              /* PWM configured as IRQ */	   
	
	// ADC0 interrupt service routine configuration
	VICVectAddr1 = (unsigned) ADC0_ISR;                    /* EINT1 ISR Address */
	VICVectCntl1 = (0x00000020 | 18);                      /* Enable EINT1 IRQ slot */
	VICIntEnable = VICIntEnable | 0x00040000;              /* Enable EINT1 interrupt */
	VICIntSelect = VICIntSelect | 0x00000000;              /* EINT1 configured as IRQ */

	AD0CR = 0x00200402;                                    /* ADC converter is operational, 10-bits, 11 clocks for conversion */
	AD0INTEN = 0x00000002;                                 /* completion of adc conversion will generate an interrupt Enable AD0.1 interrupt */
	PWMTCR = 0x02;                                         /* Reset and disable counter for PWM */
	PWMPR = 0x0E;                                          /* Prescale Register value */
	PWMMR0 = 100000;                                       /* Time period of PWM wave */
	PWMMR3 = 30000;	                                       /* Ton of PWM wave */
	PWMMCR = 0x00000203;	                                 /* Reset and interrupt on MR0 match, interrupt on MR3 match */
	PWMLER = 0x09;	                                       /* Latch enable for PWM3 and PWM0 */
	PWMPCR = 0x0800 | 1<<11;	                             /* Enable PWM3 and PWM0, single edge controlled PWM */
	PWMTCR = 0x09;	                                       /* Enable PWM and counter */
	
	while(1)
	{			
			AD0CR = AD0CR | (1<<24);                           /* Start ADC Conversion */
			percent = (result/1023.0);
			PWMMR3 = (int)(percent * 100000);
			PWMLER = 0x08;	
	}
}



