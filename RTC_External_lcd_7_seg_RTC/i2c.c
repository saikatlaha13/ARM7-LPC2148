#include <lpc214x.h> 
#include "i2c.h"
#include "lcd.h"
#include <STRING.h>
#include "delay.h"

extern int flag;
unsigned int m=0;
unsigned char *I2CData;			
unsigned char GetTmDtVal[10]={0};			   	   
unsigned char I2Counter,I2CAddress,MemAddress,Lock=0;					
unsigned char RSEC,RMIN,RHOUR,RDOW,RDATE,RMONTH,RYEAR;

void I2cInit()
{
   
	PINSEL2=0x00000000;
	IODIR0=0x70000000;
	IODIR1=0x00FF0000;
	

	VICVectCntl1 = 0x00000029;  				
	VICVectAddr1 = (unsigned)I2cISR;			
	VICIntEnable =0x00000200;	
	PINSEL0|=0x50;   	
//	I2C0SCLH 		 =234;	
  I2C0SCLH=234;
  I2C0SCLL=234;	
//	I2C0SCLL 		 =234;	
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
		
		case (0x08):											    //Start bit has been transmitted and 0x08 acknowledgement received
			if(flag==0)
			{
						I2C0CONCLR 	= 0x20;						//Clear start bit
						I2C0DAT   	= I2CAddress; 		//Send address and write bit
			}
			break;
		
		case (0x18):											    //Slave address+W, acnowledgement 0x18 has been received
			if(flag==0)
			{
						I2C0DAT   	= MemAddress;			//Write Memory start address to tx register
			}
			break;
		
		case (0x20):											    //Salve address +W, Not ACK
			if(flag==0)
			{
						I2C0DAT   	= I2CAddress; 		//Resend address and write bi
			}
			break;
		
		case (0x28):
   	if(flag==0)
		{
			      if(I2Counter-->0)			//Data sent, Ack
						{
							I2C0DAT   	= *I2CData;		  //Write data to tx register
							I2CData++;
						}
						else
						{
							I2C0CONSET 	= 0x10;					//Stop condition
							Lock = 0;                   //Signal end of I2C activity
						}	
			}
			break;
		
		case (0x30)	:											    //Data sent, NOT Ack
			if(flag==0)
			{
						I2C0DAT   	= *I2CData;				//Write data to tx register
			}
			break;
		
		
		//Receive byte conditions
		case (0x40) :											    //Slave Address +R, ACK
			if(flag==0)
			{
						I2C0CONSET 	= 0x04;						//Enable ACK for data byte
			}
			break;
		
		case (0x48) :											    //Slave Address +R, Not Ack
			if(flag==0)
			{
						I2C0CONSET 	= 0x20;					  //Resend Start condition
			}
			break;
		
		case (0x50) :											    //Data Received, ACK 
			if(flag==0)
			{
						if(--I2Counter>0)
						{
							*I2CData= I2C0DAT;   
							GetTmDtVal[m++]=*I2CData;
							I2CData++;
						}
						else
						{
							I2C0CONSET 	= 0x10;					//Stop condition
							Lock 		= 0;                //Signal end of I2C activity
							m=0;							
						}
			}
			break;
		
		case (0x58):											    //Data Received, Not Ack
			if(flag==0)
			{
						I2C0CONSET 	= 0x20;						// Resend Start condition
			}
			break;
		
		default : 		
			break;
	} 	
	//if(flag==0)
	//{
	   I2C0CONCLR 	= 0x08;											//Clear I2C interrupt flag
	   VICVectAddr = 0x00000000;								//Clear interrupt in   
	///}
	//else
	//{
//	}
}

void i2c_stop(void) 
{
	  I2C0CONSET      |= 0x00000014;     // stop bits and set AA bit
	  I2C0CONCLR       = 0x00000008;	   //SIC I2C interrupt clear bit   		 
}


void LCD_DATA(unsigned char value)
{
	  i2c_write(0x04 | (0x09 | (0xF0 & value)), DATA_WR_STAT);
    Delay_ms(1);
    i2c_write(0xF9 & (0x09 | (0xF0 & value)), DATA_WR_STAT);
    Delay_ms(1);
    i2c_write(0x04 | (0x09 | (0xF0 & value<<4)), DATA_WR_STAT);
    Delay_ms(1);
    i2c_write(0xF9 & (0x09 | (0xF0 & value<<4)), DATA_WR_STAT);
    Delay_ms(1);
}

void LCD_CMD(unsigned char value)
{
	  i2c_write(0x04 | (0x08 | (0xF0 & value)), DATA_WR_STAT);    // register select RS=1  0x08 = write  R/W=1
	  Delay_ms(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value)), DATA_WR_STAT);    // 0x04 = making RS=1
	  Delay_ms(1);
	  i2c_write(0x04 | (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);  // writing data with R/W=1
	  Delay_ms(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);
	  Delay_ms(1);	
}        


void i2c_start()
{
	  I2C0CONSET      |= 0x00000020;	  //   enabling the start bit    
    // check for a start condition has been transmitted	
	  while (I2C0STAT	!= 0x00000008);	  //   waiting for conformation and acknowledgement from slave
}

void i2c_write(char dat, char status) 
{
	  I2C0DAT          = dat;            // address
	  I2C0CONSET       = 0X00000004;	   // setting AA bit
	  I2C0CONCLR       = 0X00000028;     // setting SIC and Start flag clear bit  
	  while (I2C0STAT	!=status);		     // checking that data is transmitted and acknowledgement from slave
}

void i2c_init(void)
{
	  PINSEL0        = 0x50;          // selecting alternate functions SCL0 and SDA0
	  I2C0CONCLR       = 0x0000002C;		// clearing start bit sta  and recieve flag (SI)		
	  I2C0CONSET       = 0x00000044;	  // to set I2EN and AA bit		    
	  I2C0SCLL         = 234;			  /* bitrate = 400kHz	 */	// 70,5	  
	  I2C0SCLH         = 234; 			      
}



