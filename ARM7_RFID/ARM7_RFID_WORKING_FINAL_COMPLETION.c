#include <LPC214x.h>
#include <string.h>
#include "lcd.h"
#include "delay.h"

#define  light1  (1<<15)
#define  fan1    (1<<16)
#define  switch1 (1<<20)

void Uart0PutCh (unsigned char ch);
void Uart1PutCh (unsigned char ch);
void card_auth_response(int ,int);
void card_read_response(int,int);
void card_select_response(int,int);
void send_command(unsigned char *arr,int m,int final);
unsigned char card_data[30];
unsigned char *str;

void Delay_Ms(unsigned int count);
void Uart1PutS(unsigned char *str);
void Init_UART0(void);
void Init_UART1(void);
void Init_UART1_9600(void);
void CLEAR_FIFO1 (void);

unsigned char lcd_string1[] = "YES";
unsigned char lcd_string2[] = " NO";
unsigned char lcd_string3[] = "NO CARD ";
unsigned char lcd_string4[] = "YES CARD ";
unsigned char lcd_string5[] = "YES AUTH  ";
unsigned char lcd_string6[] = "NO AUTHEN ";
unsigned char rx_data[];
unsigned char rx_data_tst[500];
unsigned char rx_length[2];
unsigned char rx_data_tst1[500];
unsigned char rx_data1 = 'd';	
unsigned char rx_data2, AA_data;


// header part = 2 Bytes = 1st byte and 2nd byte = 0XAA,0XBB
// length part = 2 bytes = 3rd byte and 4th byte = 0x05,0x00
// node id     = 2 bytes = 5th byte and 6th byte = 0x00,0x00  #broadcast to each other
// Mifare card select = 2 bytes = 6th byte and 7th byte = 0x05,0x02
// 8 th byte = xor result of 6th and 7th byte
                       
// BYTE NO					                     #0		#1	 #2		#3   #4   #5   #6   #7   #8
unsigned char CARD_SELECT[9] =          {0xAA,0xBB,0x05,0x00,0x00,0x00,0x05,0x02,0x07};
unsigned char CARD_SELECT_response[10];

//                                                      #0   #1   #2   #3   #4   #5   #6
unsigned char Baud_9600[] =             {0xAA,0xBB,0x06,0x00,0x00,0x00,0x01,0x01,0x01,0x01};

unsigned char AUTH_CARD[] =             {0xAA,0xBB,0x0D,0x00,0x00,0x00,0x07,0x02,0x60,0x0A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x6F};
unsigned char CARD_READ[] =             {0xAA,0xBB,0x06,0x00,0x00,0x00,0x08,0x02,0x0A,0x00};
unsigned char AUTH_CARD_response[20];
unsigned char CARD_READ_response[30];

unsigned int i,l,m,k,p;
unsigned char rx_data2=0, rxdat=0, rxdat1=0;
void send_msg(unsigned char *ptr4);
 

/*---------------------------------------------------------------------------------
                             GSM Sending AT Commands
----------------------------------------------------------------------------------*/

int main(void)
{	
		IODIR1=0xF0000000;                // eg(12-15)(8-11)(4-7)(0-3)
	  IODIR0=0xffffffff;                // port 0 output
		
		IOCLR0=light1;                    // lights are switched on
		IOCLR0=fan1;                      // fans are switched on
	
		LcdInit();                        // lcd initialisation
		LcdClr();                   
	
		LcdGoToXY(0,0);						  			// Cursor position is set to first position
		LcdString("Assisi");					    // Display string content
		LcdGoToXY(1,0);				   					// Move The cursor position to the 2 line 3 position
		LcdString("Test2");	  
 	 
    Init_UART0();	                    // initializing uart 0
		Delay_Ms(100);	
		Init_UART1();                     // initialising uart 1
    Delay_Ms(100);
	
	  // baud rate command for RFID
	  for(p=0; p < 10; p++)
		{
			Uart1PutCh (Baud_9600[p]);      // setting the baud rate in RFID
		}	
		Delay_Ms(250);
	  Init_UART1_9600();
		Delay_Ms(100);
	
		VICVectCntl2 = 0x26;	 //5th bit must 1 to enable the slot
		VICIntEnable |= 0x00000040;    //enable UART0 interrupt
		VICVectCntl3 = 0x27;       
		VICIntEnable |= 0x00000080;		 //enable UART1 interrupt

		Delay_Ms(100);		
				
	  while(1)
    {
      Uart0PutCh('1');			
			for(p=0; p <9; p++)
			{
				Uart1PutCh (CARD_SELECT[p]);                                  //***** card selection ***
			}	
			if(U1IIR & 0x04)                                                // receive data available
			{
				AA_data = U1RBR;
				
				if(AA_data == 0xAA)                                           // storing the response from module to array
				{
					k=1;
					Uart0PutCh('2');
					card_select_response(k,10);
					Uart0PutCh('3');
					
					if(CARD_SELECT_response[8] == 0xF4)                         //***** no card***
					{
					    LcdGoToXY(1,0);									                        // Move The cursor position to the 2 line 3 position
						  LcdString(lcd_string3);
						  Uart0PutCh('4');		
					}
					else                                                        //***** if there is a card***
					{
						  Uart0PutCh('5');		
					    LcdGoToXY(1,0);									                        // Move The cursor position to the 2 line 3 position
					    LcdString(lcd_string4);                                 //***** yes card***
						  Delay_Ms(200);
						  CLEAR_FIFO1 ();
						
						  for(p=0; p <17; p++)
				      {
				        Uart1PutCh (AUTH_CARD[p]);                            //***** authenticate card ****
				      }
              Uart0PutCh('6');									
					    k=0;
							card_auth_response(k,10);
							Uart0PutCh('7');		
				      
				      if(AUTH_CARD_response[9] == 0x00)                       //***** if response contains 0x00***
						  {
								 Uart0PutCh('8');		
								 LcdGoToXY(1,0);									                    // Move The cursor position to the 2 line 3 position
								 LcdString(lcd_string5);                              //**** yes authentication ***
								 Delay_Ms(200);
						     CLEAR_FIFO1 ();
									
						     for(p=0; p <10; p++)
							   {
								   Uart1PutCh (CARD_READ[p]);                         //***** card read***
							   }
                 Uart0PutCh('9');										 
							   k=0;
								 
								 // reading card read command response
								 card_read_response(k,26);
								 LcdCmd4Bit(0x80);
							   for (p=19; p<26; p++)
							   {
									  LcdChar4Bit(CARD_READ_response[p]);
					 		   }
								 Delay_Ms(4000);
								 LcdClr();
					    }		
					    else                                                     // ***** no authentication ***
					    {
					      LcdGoToXY(1,0);									                       // Move The cursor position to the 2 line 3 position
					      LcdString(lcd_string6);                                // ***** no authentication ***
						    Delay_Ms(200);
						    CLEAR_FIFO1 ();
					    }
					}			
				}
			}									
	  }
}

void CLEAR_FIFO1 (void)
{
   U1FCR = 0x06; // clear FIFO Register
   U1FCR = 0x01;
}

void Init_UART0(void)
{
		PINSEL0 |= 0x00000005;          //Enable RxD0 and TxD0             
    U0LCR = 0X83;                   //: Used to configure the UART block (i.e the data format used in transmission)
    U0DLL = 97;                     // 9600 Baud Rate @ 15MHz VPB Clock      
    U0LCR = 0x03;
		U0FCR = 0x06;		  
		U0FCR = 0x01;
    U0IER = 0x03;                   //RI and TI interrupt
}

void Init_UART1(void)
{
		PINSEL0 |= 0x00050000;          //Enable RxD0 and TxD0             
    U1LCR = 0X83;
		U1DLL = 49;                     // 9600 Baud Rate @ 15MHz VPB Clock      
    U1LCR = 0x03;
		U1FCR = 0x06;
		U1FCR = 0x01;
    U1IER = 0x03;                   //RI and TI interrupt
}

// initializing uart1 with baud rate of 9600
void Init_UART1_9600(void)
{
		PINSEL0 |= 0x00050000;           // Enable RxD0 and TxD0             
    U1LCR = 0X83;                    // 8 bit character length enable access to divisior latch
		U1DLL = 97;                      // 9600 Baud Rate @ 15MHz VPB Clock      
    U1LCR = 0x03;                    
		U1FCR = 0x06;                    // resetting pointer logic
		U1FCR = 0x01;                    // active high enable for both uart tx and rx
    U1IER = 0x03;                    // RI and TI interrupt
}

void Uart0PutCh (unsigned char ch)   // Write character to Serial Port   
{                  		
   while (!(U0LSR & 0x20));	         // wait till the THR is empty                                    
	                                   // now we can write to the Tx FIFO
  	U0THR = ch;
}

void Uart0PutS(unsigned char *str)	 //A function to send a string on UART0
{  
   while(*str)
   {  
      Uart0PutCh(*str++);	    
   }
}

void Uart1PutCh (unsigned char ch)   // Write character to Serial Port   
{                  		
   U1THR = ch;
	 while (!(U1LSR & 0x20));	  // wait till the THR is empty
}

void  Uart1PutS(unsigned char *str)	 //A function to send a string on UART1
{  
   while(*str)
   {  
      Uart1PutCh(*str++);	    
   }
}

void Delay_Ms(unsigned int count)
{
    volatile unsigned int j,k;
    for (j=0;j<count;j++)
	  {
        for (k=0;k<6000;k++)
		    {
            __asm
			     {
                nop;
                nop;
           }
        }
    }
}

void card_select_response(int k,int final)
{
	  while(k<final)
	  {
			if(U1IIR & 0x04)                   
			{
				CARD_SELECT_response[k] = U1RBR;                        //*****Card select response*** 
		    k++;
			}
		}
		//return;
}

void card_read_response(int k,int final)
{
	while (k<final)
	{
		 if(U1IIR & 0x04)
	   {
			 CARD_READ_response[k]= U1RBR;                   // ***** response to card read command***
			 k++;
		 }
	}
	//return;
}

void card_auth_response(int k,int final)
{
	while (k<final)
	{
		 if(U1IIR & 0x04)
		 { 
				AUTH_CARD_response[k]= U1RBR;                       //***** authentication response from reader***
				k++;
		 }
	}
	//return;
}



