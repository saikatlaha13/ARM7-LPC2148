#include <LPC21xx.H>
#include <STRING.H>

unsigned int operation(unsigned int value);

#define DEV_WR_STAT  0x18	 // to confrm the ack fr device address
#define DATA_WR_STAT 0x28	  //  to confrm the ack fr data
#define DEV_ADDR    0x4E	  //   device address

unsigned int j,i,k,l;
unsigned int a[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	
unsigned int b[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned int c[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	
unsigned int d[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	
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

int main()
{
	int i,j;
  PINSEL0=0x00000000;		  		//Making All ports as GPIO pins
	PINSEL1=0x00000000;
	IODIR0=0x000F0000;			    //P0.0-P0.3 Set as Output Port
	IODIR1=0x00FF0000;			    //P1.16-P1.23 Set as Output Port , 7seg connected	
	
	i2c_init();
	i2c_start();
	i2c_write(DEV_ADDR ,DEV_WR_STAT);
	  
		//LCD_INIT();
	while(1)
	{
		for(i=0 ; i<10000 ; i++)
		{
			//for(j=0 ; j<3000 ; j++);
			for(j=0 ; j<40 ; j++)
			{
				display(i);
			}
		}
		//display();
	  
	  //LCDSTR(0xC0,"I2C TEST PRG");
	}
}

void DELAY(unsigned int VALUE)
{
      unsigned int i,j;
      for(i=0;i<VALUE;i++)
      {
          //for(j=1;j<1200;j++);
				  //for(j=1 ; j<7750 ; j++);
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
		i2c_write(operation(d[thousand]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00010000;
	
	  IOSET0=0x00020000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(c[hundred]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00020000;
						
		IOSET0=0x00040000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(a[tens]),DATA_WR_STAT);
		DELAY(2);
		IOCLR0=0x00040000;
				
		IOSET0=0x00080000;
		IOCLR1=0X00FF0000;
		i2c_write(operation(b[ones]),DATA_WR_STAT);
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

