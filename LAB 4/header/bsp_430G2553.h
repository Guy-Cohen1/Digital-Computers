#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx


#define   debounceVal      250


// RGB led abstraction
#define RGBArrPortOut      P2OUT
#define RGBArrPortSel      P2SEL
#define RGBArrPortDir      P2DIR

// POT abstraction
#define POTArrPortIN       P1IN
#define POTArrPortSel      P1SEL
#define POTArrPortDir      P1DIR

// UART abstraction
#define UARTArrPortOUT     P1OUT
#define UARTArrPortSel     P1SEL
#define UARTArrPortSel2    P1SEL2
#define UARTArrPortDir     P1DIR

/*
// PushButtons abstraction
#define PBsArrPort	       P1IN
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 


// PushButton3 abstraction
#define PB3ArrPort	   P2IN 
#define PB3ArrIntPend	   P2IFG 
#define PB3ArrIntEn	   P2IE
#define PB3ArrIntEdgeSel   P2IES
#define PB3ArrPortSel      P2SEL 
#define PB3ArrPortDir      P2DIR 

//genrator abstraction
#define GENsArrPortIn	    P2IN 
#define GENsArrPortDir      P2DIR 
#define GENsArrPortSel      P2SEL
#define GENsArrPortOut	    P2OUT 

#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x01

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
extern void UARTconfig(void);

#endif
