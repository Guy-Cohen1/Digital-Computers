#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp_msp430x2xx.h"    		// private library - BSP layer
#include  "../header/app.h"    		                // private library - APP layer
//#include  "../header/bsp_msp430x4xx.h"   		// private library - BSP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable


extern void sysConfig(void);
extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void print2_7(int num);
extern void flag_off(int pin);

extern __interrupt void PBs_handler(void);

#endif







