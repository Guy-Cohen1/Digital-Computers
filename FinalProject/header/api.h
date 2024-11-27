#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void Joystick_sample_for_paint();
extern void JoyStickADC_Steppermotor();
extern void rotate_clock_direction(long);
extern void rotate_anti_clock_direction(long);
extern void rotate_clock_direction_calibaration(long);
extern void blinkRGB();
extern void clear_counters();
extern void change_delay_time();
extern void calibrate();
extern void Script_mode_handler();
extern void start_execution();
extern void ClearLEDsRGB();
extern void countUP();
extern void countDOWN();
extern void rra_LCD(int num);
extern void rst_count_lcd_clr();

extern int16_t Vrx;
extern int16_t Vry;






#endif







