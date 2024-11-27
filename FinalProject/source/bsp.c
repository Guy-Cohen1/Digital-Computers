#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT

  // JoyStick Configuration  P1.3 - Vrx; P1.4 - Vry; P1.5 - PB
  // P1.3-P1.4 - X(Don't care) for Sel, Dir According the dataSheet For A3,A4 input Select For ADC
  JoyStickPortSEL &= ~BIT5;  // P1.5 Sel = '0'
  JoyStickPortDIR &= ~BIT5;  // P1.5 input = '0'
  JoyStickPortOUT &= ~BIT5;  //
  JoyStickIntEN |= BIT5; // P1.5 PB_interrupt = '1'
  JoyStickIntEdgeSel |= BIT5; // P1.5 PB pull-up - '1'
  JoyStickIntPend &= ~BIT5; // Reset Int IFG - '0'

  // Stepmotor Configuration
  StepmotorPortSEL &= ~(BIT0+BIT1+BIT2+BIT3);  // P2.0-P2.3 Sel = '0'
  StepmotorPortDIR |= BIT0+BIT1+BIT2+BIT3;  // P2.0-P2.3 output = '1'

  // LCD Setup
  LCD_DATA_WRITE &= ~0xFF;      // Clear LCD data output
  LCD_DATA_DIR |= 0xF0;         // Set LCD data pins as outputs (upper 4 bits)
  LCD_DATA_SEL &= ~0xF0;        // Select GPIO function for LCD data
  LCD_CTL_SEL &= ~0xC1;         // Select GPIO function for LCD control


  _BIS_SR(GIE);                     // enable interrupts globally
}
//-------------------------------------------------------------------------------------
//            Stop All Timers
//-------------------------------------------------------------------------------------
void StopAllTimers(void){
    TACTL = MC_0; // halt timer A
}

//-------------------------------------------------------------------------------------
//            Timer A  configuration
//-------------------------------------------------------------------------------------
void TIMER_A0_config(unsigned int counter){
    TACCR0 = counter;
    TACCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;  //  select: 2 - SMCLK ; control: 1 - Up ; divider: 3 - /8 (2^20/8)
    TA0CTL |= TACLR;
}


//-------------------------------------------------------------------------------------
//                              UART init
//-------------------------------------------------------------------------------------
void UART_init(void){
    if (CALBC1_1MHZ==0xFF)
      {
        while(1);
      }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
   // P1DIR |= RXLED + TXLED;
    P1OUT &= 0x00;

    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;
}


//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADC10CTL1 = INCH_4 + CONSEQ_1 + ADC10SSEL_3;            // A4/A3 highest highest channel for a sequence of conversions
    ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE;
    ADC10DTC1 = 0x02;                         // 2 conversions
    ADC10AE0 |= 0x18;                         // P1.3-4 ADC10 option select
}

