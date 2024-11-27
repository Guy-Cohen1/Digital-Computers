#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      2000
#define     HIGH    1
#define     LOW     0

// Joystick abstraction
#define JoyStickPortOUT     P1OUT
#define JoyStickPortSEL     P1SEL
#define JoyStickPortDIR     P1DIR
#define JoyStickPortIN      P1IN
#define JoyStickIntEdgeSel  P1IES
#define JoyStickIntEN       P1IE
#define JoyStickIntPend     P1IFG


// Stepmotor abstraction
#define StepmotorPortOUT     P2OUT
#define StepmotorPortSEL     P2SEL
#define StepmotorPortDIR     P2DIR


//    LCD abstraction

#define LCD_DATA_WRITE    P2OUT
#define LCD_DATA_DIR      P2DIR
#define LCD_DATA_SEL      P2SEL
#define LCD_CTL_SEL       P1SEL //control signals
#define LCD_DATA_READ     P2IN


#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1


extern void GPIOconfig(void);
extern void ADCconfig(void);
extern void TIMER_A0_config(unsigned int counter);
extern void TIMER_A1_config(unsigned int counter);
extern void StopAllTimers(void);
extern void UART_init(void);




#endif



