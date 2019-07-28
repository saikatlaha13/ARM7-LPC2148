#include <LPC21xx.H>
#include <STRING.H>

#define DEV_WR_STAT  0x18	 // to confrm the ack fr device address
#define DATA_WR_STAT 0x28	  //  to confrm the ack fr data
#define DEV_ADDR    0x4E	  //   device address

void DELAY(unsigned int);

void LCDSTR(unsigned char ADDRESS,char *MSG);
void LCD_DATA(unsigned char);
void LCD_CMD(unsigned char);
void LCD_INIT(void);

void i2c_init(void);
void i2c_start(void);
void i2c_write(char dat, char status); 
void i2c_stop(void);

unsigned char I2Cdata;

int main()
{	        
	  i2c_init();
	  i2c_start();
	  i2c_write(DEV_ADDR ,DEV_WR_STAT);
	  
		//LCD_INIT();
	while(1)
	{
	  i2c_write(0x07,DATA_WR_STAT);
	  //LCDSTR(0xC0,"I2C TEST PRG");
	}
}

void DELAY(unsigned int VALUE)
{
      unsigned int i,j;
      for(i=0;i<VALUE;i++)
      {
          for(j=1;j<1200;j++);
      }
}

/*void LCDSTR(unsigned char ADDRESS,char *MSG)
{	          
         unsigned char COUNT,LENGTH;
		     LCD_CMD(ADDRESS);
         LENGTH = strlen(MSG);
             for(COUNT=0;COUNT<LENGTH;COUNT++)
               {
                  LCD_DATA(*MSG); 				  
                  MSG++;
               }
}*/

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


	/*void LCD_CMD(unsigned char value)
  {
	  i2c_write(0x04 | (0x08 | (0xF0 & value)), DATA_WR_STAT);
	  DELAY(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value)), DATA_WR_STAT);
	  DELAY(1);
	  i2c_write(0x04 | (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);
	  DELAY(1);
	  i2c_write(0xF9 & (0x08 | (0xF0 & value<<4)), DATA_WR_STAT);
	  DELAY(1);	
  }        

	void LCD_DATA(unsigned char value)
  {
		//value=0x31;
	  i2c_write(0x04 | (0x09 | (0xF0 & 0x32)), DATA_WR_STAT);
      DELAY(1);
      i2c_write(0xF9 & (0x09 | (0xF0 & 0x32)), DATA_WR_STAT);
      DELAY(1);
      i2c_write(0x04 | (0x09 | (0xF0 & 0x32<<4)), DATA_WR_STAT);
      DELAY(1);
      i2c_write(0xF9 & (0x09 | (0xF0 & 0x32<<4)), DATA_WR_STAT);
      DELAY(1);
  }

	
void LCD_INIT()
  {	
      LCD_CMD(0x28);
      LCD_CMD(0x0C);
      LCD_CMD(0x01);
      LCD_CMD(0x06);
  }*/	

	 



