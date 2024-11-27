#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer
#include <string.h>

extern void findHz();
extern void NumToString(int num);
extern void UpPauseCount();
extern void timerA0On();
extern void buzzer_gen();
void time_to_lcd(int sec);
float getMultiplicativeFactor(float freq);
void Real_Time(char* first1,char* first2,char* last1,char* last2);
void twosectimecount();
extern void Signal_shape();

#endif