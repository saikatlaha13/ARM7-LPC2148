#ifndef LCD_LPC2129_H
#define LCD_LPC2129_H

void LcdCmd4Bit(unsigned char Cmd);	
void LcdInit(void);
void LcdClr(void);	
void LcdGoToLine1(void);	
void LcdGoToLine2(void);	 
void LcdChar4Bit(unsigned char Chr);
void LcdString(unsigned char *Str);
void LcdScrollMsg(unsigned char *msg_ptr);
void LcdDisplayNumber(unsigned int num);
void LcdGoToXY(unsigned char row,unsigned char col);
void LcdDispRtcTime(unsigned hour, unsigned char min, unsigned char sec);
void LcdDispRtcDate(unsigned day, unsigned char month, unsigned char year);

#endif


