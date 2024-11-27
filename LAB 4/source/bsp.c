#include  "../header/bsp_430G2553.h"

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  if (CALBC1_1MHZ==0xFF)
  {
    while(1);
  }
  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

/*
  // PushButtons Setup
  PBsArrPortSel &= ~0x07;
  PBsArrPortDir &= ~0x07;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x04;         // pull-down mode
  PBsArrIntEn |= 0x07;
  PBsArrIntPend &= ~0x07;            // clear pending interrupts 
  
  */

  // RGB led Setup
  RGBArrPortOut        &= ~0x07;
  RGBArrPortSel        &= ~0x07;
  RGBArrPortDir        |= 0x07;

  // POT Setup
  //POTArrPortIN         &= ~0x08;
  POTArrPortSel        &= ~0x08;
  POTArrPortDir        &= ~0x08;
  
  //buzer Setup 
   BUZER_OUT           &= ~0x10;
   BUZER_DIR           |= 0x10;
   BUZER_SEL           |= 0x10;
   
  //LCD Setup 
   LCD_DATA_WRITE      &= ~0xFF;
   LCD_DATA_DIR        |= 0xF0;
   LCD_DATA_SEL        &= ~0xF0;
   LCD_CTL_SEL         &= ~0xE0;
  
  _BIS_SR(GIE);                     // enable interrupts globally
}

//-------------------------------------------------------------------------------------
//            UART congiguration
//-------------------------------------------------------------------------------------

void UARTconfig(void){
  UARTArrPortSel |= BIT1 + BIT2;    // Select peripheral function for UART pins
  UARTArrPortSel2 |= BIT1 + BIT2;   // Select secondary peripheral function for UART pins
  // UARTArrPortDir |= BIT1 + BIT2; // Set UART pins as outputs (uncomment if needed)
  // UARTArrPortOUT &= ~(BIT1 + BIT2); // Clear output bits for UART pins (uncomment if needed)
  UCA0CTL1 |= UCSSEL_2;             // Select SMCLK as clock source for UART
  UCA0BR0 = 109;                    // Set baud rate to 9600 (assuming 1 MHz clock)
  UCA0BR1 = 0x00;                   // Set baud rate to 9600 (assuming 1 MHz clock)
  UCA0MCTL = UCBRS0;                // Set modulation (UCBRSx = 1)
  UCA0CTL1 &= ~UCSWRST;             // Initialize USCI state machine
  IE2 |= UCA0RXIE;                  // Enable USCI_A0 RX interrupt
}


//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
        
	TA0CTL = TASSEL_2 + TACLR + ID_3;  // timer is SMCLK /8 = 0.5 sec interupt in compare mode
    TA0CCTL0 = CCIE;    //interupt enable
    TA0CCR0 = 0xFFFF;

    TA1CTL = TASSEL_2 +TACLR;        // SMCLK
    TA1CCTL2 = OUTMOD_0;

} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	ADC10CTL0 &= ~0x0002;
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;   //0-3v, sample rate= 16*(1/2^20)
    ADC10CTL1 = INCH_3 + ADC10SSEL_3; //channel A3, SMCLK
    ADC10AE0 |= BIT3;
}              


