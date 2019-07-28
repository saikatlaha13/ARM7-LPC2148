#include <lpc214x.h> 
#include "i2c.h"		

unsigned int m=0;
unsigned char *I2CData;			
unsigned char GetTmDtVal[10]={0};			   	   
unsigned char I2Counter,I2CAddress,MemAddress,Lock=0;					
unsigned char RSEC,RMIN,RHOUR,RDOW,RDATE,RMONTH,RYEAR;
void I2cInit()
{	

	VICVectCntl1 = 0x00000029;  				
	VICVectAddr1 = (unsigned)I2cISR;			
	VICIntEnable =0x00000200;	
	PINSEL0|=0x50;   	
	I2C0SCLH 		 =234;							
	I2C0SCLL 		 =234;	
}	
														  
void ExtractData(unsigned char *ptr)
{
	unsigned int ExDt;		
	for(ExDt=0;ExDt<7;ExDt++)
	{  	
		switch(ExDt)
		{ 
			case 0:		RSEC=ptr[ExDt];
						break;
			case 1:		RMIN=ptr[ExDt];
						break;
			case 2:		RHOUR=ptr[ExDt];
						break;
			case 3:		RDOW=ptr[ExDt];
						break;
			case 4:		RDATE=ptr[ExDt];
						break;
			case 5:		RMONTH=ptr[ExDt];
						break;
			case 6:		RYEAR=ptr[ExDt];
						break;	
			default: 	break;
		}			
	} 
}

void I2cTransferByte(unsigned int I2CAddr,unsigned char MemAddr,unsigned char count,unsigned char *ap)
{ 
	while(Lock == 1);											//Wait for interrupt to signal end of I2C activity	  
	Lock 		= 1;                   				//Set I2C bus as active
	
	I2CAddress 	= I2CAddr;								//Place address and data in Globals to be used by the interrupt

	if(count >0)
	{
		I2CData  	= (unsigned char *)ap;
	}
	I2Counter	= count;
	MemAddress	= MemAddr;
	I2C0CONCLR 	= 0x000000FF;							//Clear all I2C settings
	I2C0CONSET 	= 0x00000040; 						//Enable the I2C interface
	I2C0CONSET 	= 0x00000020; 						//Start condition	   	
}

	
void I2cISR (void)	__irq									//I2C interrupt routine
{
	switch (I2C0STAT)												//Read result code and switch to next action
	{
		// Start and Send byte conditions
		
		case (0x08):											//Start bit
						I2C0CONCLR 	= 0x20;						//Clear start bit
						I2C0DAT   	= I2CAddress; 				//Send address and write bit
						break;
		
		case (0x18):											//Slave address+W, ACK
						I2C0DAT   	= MemAddress;				//Write Mem,ory start address to tx register
						break;
		
		case (0x20):											//Salve address +W, Not ACK
						I2C0DAT   	= I2CAddress; 				//Resend address and write bi
						break;
		
		case (0x28):   	if(I2Counter-->0)						//Data sent, Ack
						{
							I2C0DAT   	= *I2CData;				//Write data to tx register
							I2CData++;
						}
						else
						{
							I2C0CONSET 	= 0x10;					//Stop condition
							Lock = 0;                       	//Signal end of I2C activity
						}										 
						break;
		
		case (0x30)	:											//Data sent, NOT Ack
						I2C0DAT   	= *I2CData;					//Write data to tx register
						break;
		
		
		//Receive byte conditions
		
		case (0x40) :											//Slave Address +R, ACK
						I2C0CONSET 	= 0x04;						//Enable ACK for data byte
						break;
		
		case (0x48) :											//Slave Address +R, Not Ack
						I2C0CONSET 	= 0x20;										//Resend Start condition
						break;
		
		case (0x50) :											//Data Received, ACK 
						if(--I2Counter>0)
						{
							*I2CData= I2C0DAT;   
							GetTmDtVal[m++]=*I2CData;
							I2CData++;
						}
						else
						{
							I2C0CONSET 	= 0x10;					//Stop condition
							Lock 		= 0;                    //Signal end of I2C activity
							m=0;							
						}
						break;
		
		case (0x58):											//Data Received, Not Ack
						I2C0CONSET 	= 0x20;						// Resend Start condition
						break;
		
		default : 		break;
		
	} 	
	I2C0CONCLR 	= 0x08;											//Clear I2C interrupt flag
	VICVectAddr = 0x00000000;									//Clear interrupt in   
}
