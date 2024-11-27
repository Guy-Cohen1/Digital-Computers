#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer
//#include <string.h>
//extern sprintf;

extern void NumToString(int num);


extern void timerA0On();
extern void RGBBLINK();
extern void STOPER();
extern void buzzer();
extern void set_X();
extern void PRINT_POT();
extern void LCD_CLR_AND_RESET_X();

#endif
