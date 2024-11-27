#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx


#define   debounceVal      250


// PushButtons abstraction
#define PBsArrPort	   P1IN 
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	   P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 

// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL

//genrator abstraction
#define GENsArrPortIn	    P2IN 
#define GENsArrPortDir      P2DIR 
#define GENsArrPortSel      P2SEL
#define GENsArrPortOut	    P2OUT 

#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08
/*
#define PB0                0x10
#define PB1                0x20
#define PB2                0x40
#define PB3                0x80 */

//    LCD abstraction

#define LCD_DATA_WRITE    P1OUT
#define LCD_DATA_DIR      P1DIR
#define LCD_DATA_SEL      P1SEL
#define LCD_CTL_SEL       P2SEL
#define LCD_DATA_READ     P1IN


//   BUZER abstraction
#define BUZER_OUT         P2OUT
#define BUZER_DIR         P2DIR
#define BUZER_SEL         P2SEL


extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);

#endif
