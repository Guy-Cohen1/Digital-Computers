#ifdef __MSP430FG4619
#include "../header/bsp_430FG4619.h"
#else
#include "../header/bsp_430G2553.h"
#endif

void GPIOconfig(void){
    WDTCTL = WDTHOLD | WDTPW;
  
    PBsArrPortSel &= ~0x07;
    PBsArrPortDir &= ~0x07;
    PBsArrIntEdgeSel |= 0x03;
    PBsArrIntEdgeSel &= ~0x04;
    PBsArrIntEn |= 0x07;
    PBsArrIntPend &= ~0x07;
  
    P2SEL &= ~0x02;
    P2DIR &= ~0x02;
    P2IES &= ~0x02;
    P2IE  |= 0x02;
    P2IFG &= ~0x02;

    GENsArrPortSel |= 0x10;
    GENsArrPortDir &= ~0x10;

    BUZER_OUT &= ~0x04;
    BUZER_DIR |= 0x04;
    BUZER_SEL |= 0x04;
  
    LCD_DATA_WRITE &= ~0xFF;
    LCD_DATA_DIR |= 0xF0;
    LCD_DATA_SEL &= ~0xF0;
    LCD_CTL_SEL &= ~0xE0;
  
    P2SEL &= ~0x08;
    P2DIR |= 0x08;
  
    SWsArrPortSel &= ~0x01;
    SWsArrPortDir &= ~0x01;
    _BIS_SR(GIE);
}

void TIMERconfig(void) {
    WDTCTL = WDTPW + WDTHOLD;
    TA0CTL = TASSEL_2 + ID_3 + MC_0;
    TA0CCTL0 = CCIE;

    TA1CTL = TASSEL_2 + TACLR;
    TA1CCTL1 = OUTMOD_0;
    TA1CCTL2 = CM_1 + SCS + CAP + CCIE + CCIS_0;
    TA1CCR2 = 0;
    __bis_SR_register(GIE);
}

void ADCconfig(void){
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + SREF_0 + ADC10IE;
    ADC10CTL1 = INCH_3 + ADC10SSEL_3;
    ADC10AE0 |= BIT3;
}
