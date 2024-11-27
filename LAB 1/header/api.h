#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void printSWs2LEDs(void);
extern void printArr2SWs(char Arr[], int size, unsigned int rate);
extern void Lab(void);

char count_up_down(int *flag, char last_count);
int move_leds(int last_move);
int pwm();
#endif







