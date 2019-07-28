#include <LPC21xx.H>
#include <STRING.H>
#include <STDARG.h>
#include <stdio.h>

unsigned int operation(unsigned int value);

#define DEV_WR_STAT  0x18	  //  to confrm the ack fr device address
#define DATA_WR_STAT 0x28	  //  to confrm the ack fr data
#define DEV_ADDR     0x4E	  //  device address for seven segment
#define DEV_ADDR1    0x4A   //  device address for lcd module

unsigned int j,i,k,l;
unsigned int a[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	
	
void DELAY(unsigned int);
void LCDSTR(unsigned char ADDRESS,char *MSG);
void LCD_DATA(unsigned char);
void LCD_CMD(unsigned char);
void LCD_INIT(void);
void i2c_init(void);
void i2c_start(void);
void i2c_write(char dat, char status); 
void i2c_stop(void);
void display();
unsigned char I2Cdata;
char result[10];

int main()
{
	int i,j;
  PINSEL0=0x00000000;		  		//Making All ports as GPIO pins
	PINSEL1=0x00000000;         
	IODIR0=0x000F0000;			    //P0.0-P0.3 Set as Output Port
	IODIR1=0x00FF0000;			    //P1.16-P1.23 Set as Output Port , 7seg connected	

  while(1)
	{
		for(i=0 ; i<10000 ; i++)
		{
			for(j=0 ; j<40 ; j++)
			{
				i2c_init();
	      i2c_start();
	      i2c_write(DEV_ADDR ,DEV_WR_STAT);
				display(i);
				i2c_stop();
			}
				i2c_init();
	      i2c_start();
	      i2c_write(DEV_ADDR1 ,DEV_WR_STAT);
				LCD_INIT();
				sprintf(result,"%d",i);
	      LCDSTR(0x80,result);
	      i2c_stop();
		}
	}
}

void DELAY(unsigned int VALUE)
{
      unsigned int i,j;
      for(i=0;i<VALUE;i++)
      {
			    for(j=1 ; j<10000 ; j++);
      }
}

void i2c_init(void)
{
	  PINSEL0        = 0x50;
	  I2CONCLR       = 0x0000002C;		// clearing start bit sta  and recieve flag (SI)		
	  I2CONSET       = 0x00000044;	    //enabling the I2C, AA=assert ack		    
	  I2SCLL         = 0x0700;			            /* bitrate = 400kHz	 */	// 70,5	  
	  I2SCLH         = 0x0050; 			      
}	

void i2c_start()
{
	  I2CONSET      |= 0x00000020;	  //   enabling the start bit       
	  while (I2STAT	!= 0x00000008);	   	// waiting for conformation
}	

void i2c_write(char dat, char status) 
{
	  I2DAT          = dat;
	  I2CONSET       = 0X00000004;	
	  I2CONCLR       = 0X00000028;
	  while (I2STAT	!=status);		 
}	 

void i2c_stop(void) 
{
	  I2CONSET      |= 0x00000014;
	  I2CONCLR       = 0x00000008;	   		 
}	

void display(unsigned int i)
{
		unsigned int residue,ext,new_value,value,display_value;
		
		unsigned int ones,tens,hundred,thousand;
		ones=i%10;
		i=i/10;
		
		tens=i%10;
		i=i/10;
		
		hundred=i%10;
		i=i/10;
		
		thousand=i%10;
		
		IOSET0=0x00010000;
		i2c_write(operation(a[thousand]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00010000;
	
	  IOSET0=0x00020000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(a[hundred]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00020000;
						
		IOSET0=0x00040000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(a[tens]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00040000;
				
		IOSET0=0x00080000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(a[ones]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00080000;
}

unsigned int operation(unsigned int value)
{
	unsigned int mask,extracted_value,final_value;
	mask=(~0<<3);
	extracted_value=value & mask;
	value=value ^ extracted_value;
	final_value=value | (extracted_value<<1);
	
	return final_value;
}

void LCD_INIT()
{	
    LCD_CMD(0x28);
    LCD_CMD(0x0C);
    LCD_CMD(0x01);
    LCD_CMD(0x06);
}	

void LCD_CMD(unsigned char value)
{
	  i2c_write(0x04 | (0x08 | (0xF0 & value)), DATA_WR_STAT);    // register select RS=1  0x08 = write  R/W=1
	  DELAY(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value)), DATA_WR_STAT);    // 0x04 = making RS=1
	  DELAY(1);
	  i2c_write(0x04 | (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);  // writing data with R/W=1
	  DELAY(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);
	  DELAY(1);	
}        

void LCD_DATA(unsigned char value)
{
	  i2c_write(0x04 | (0x09 | (0xF0 & value)), DATA_WR_STAT);
    DELAY(1);
    i2c_write(0xF9 & (0x09 | (0xF0 & value)), DATA_WR_STAT);
    DELAY(1);
    i2c_write(0x04 | (0x09 | (0xF0 & value<<4)), DATA_WR_STAT);
    DELAY(1);
    i2c_write(0xF9 & (0x09 | (0xF0 & value<<4)), DATA_WR_STAT);
    DELAY(1);
}

void LCDSTR(unsigned char ADDRESS,char *MSG)
{	          
    unsigned char COUNT,LENGTH;
		LCD_CMD(ADDRESS);
    LENGTH = strlen(MSG);
    for(COUNT=0;COUNT<LENGTH;COUNT++)
    {
         LCD_DATA(*MSG); 				  
         MSG++;
    }
}

