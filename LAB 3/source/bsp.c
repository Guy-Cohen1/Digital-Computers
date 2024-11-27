#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT

  //LEDS Setup
     LEDsArrPortSel     &= ~0xFF;      //P9 to I/O
     LEDsArrPortDir     |= 0xFF;       //P9 to Output
     LEDsArrPort         = 0x00;       // clear all LEDs

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  
  // Buzzer Setup
  //BuzzPortDir |= BIT2;             // P2.2 Output compare - '1'
  //BuzzPortSel |= BIT2;             // P2.2 Select = '1'
  //BuzzPortOut &= ~BIT2;             // P2.2 out = '0'

  // Keypad Setup
  KeypadPortSel &= ~0xFF;
  KeypadPortDIR = 0x0F; //10.0-10.3 output, 10.4-10.7 input
  KeypadPortOUT = 0x00; // CLR output

  // Keypad IRQ Setup
  KeypadIRQPortSel &= ~BIT1;
  KeypadIRQPortDir &= ~BIT1;             // P2.1 input
  KeypadIRQIntEdgeSel |= BIT1;         // pull-up mode  P2.1 - '1'
  KeypadIRQIntEn |= BIT1;               // P2.1 - '1'
  KeypadIRQIntPend &= ~0xFF;            // clear pending interrupts P2.1

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;            //
  PBsArrPortOut &= ~0x07;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x07;            // P1.0-2 - Input ('0')
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x07;               // P1.0-2 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1
  
// PushButton 3 Setup For Main Lab
   PB3sArrPortSel &= ~BIT3;            //
   PB3sArrPortOut &= ~BIT3;            // Set P2Out to '0'
   PB3sArrPortDir &= ~BIT3;            // P2.0 - Input ('0')
   PB3sArrIntEdgeSel &= ~BIT3;         // pull-down mode  P2.0 - '0'
   PB3sArrIntEn |= BIT3;               // P1.0-2 - '1'
   PB3sArrIntPend &= ~BIT3;            // clear pending interrupts P2.0

  _BIS_SR(GIE);                     // enable interrupts globally


}

//-------------------------------------------------------------------------------------
//            Timer A 345msec configuration
//-------------------------------------------------------------------------------------
void TIMER_A0_config(void){
   // WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TACCR0 = 0xFFFF; // (2^20/8)*345m = 45219 -> 0xB0A3
    TACCTL2 = 0;
    TACCR2 = 0;
    TACCTL0 = CCIE + TACLR;
    TA0CTL = TASSEL_1 + MC_1 + ID_0;  //  select: 2 - SMCLK ; control: 1 - Up ; divider: 3 - /8
}





//-------------------------------------------------------------------------------------
//            Stop All Timers
//-------------------------------------------------------------------------------------
void StopAllTimers(void){

    TACTL = MC_0; // halt timer A
    TBCTL = MC_0; // halt timer B
    TBCCTL1 = 0x00; // stop PWM
    DMA0CTL = 0; // Stop DMA0


}
//-------------------------------------------------------------------------------------
//            Timers configuration
//-------------------------------------------------------------------------------------
void TIMERconfig(void) {
    TBCCR0 = 0xFFFF; // (2^20/8)*345m = 45219 -> 0xB0A3
    TBCCTL0 = CCIE + TACLR;
    TB0CTL = TASSEL_1 + MC_1 + ID_0;  //  select: 2 - SMCLK ; control: 1 - Up ; divider: 3 - /8
}
//-------------------------------------------------------------------------------------
//            DMA configuration
//-------------------------------------------------------------------------------------
void DMA_config(void){
    DMACTL1 = DMA1TSEL_8; // secound turnes on by timer b compare
    //DMA0CTL = DMADT_1 + DMASBDB + DMAEN + DMAIE + DMADSTINCR_3 + DMASRCINCR_3; // block transfer mode , byte-to-byte , enable
    DMA1SZ = 0x004;
    DMA1CTL = DMADT_1 + DMASBDB + DMAIE; // block mode , byte-to-byte , enable
    DMA1DA = (unsigned int)&LEDsArrPort;
}



