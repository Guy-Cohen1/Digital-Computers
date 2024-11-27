#include  "../header/bsp_msp430x2xx.h"    // private library - BSP layer
//#include  "../header/bsp_msp430x4xx.h"
//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;				  // clear all LEDs
  
  // Switches Setup
  //SWsArrPortSel &= ~0x0F;
  //SWsArrPortDir &= ~0x0F;
  
  // PushButtons Setup
  PBsArrPortSel &= ~0xFF;
  PBsArrPortDir = 0x80;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0xFC;         // pull-down mode
  PBsArrIntEn |= 0x7F;
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts 
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	
	//write here timers congiguration code
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	//write here ADC congiguration code
}              

           
             

 
             
             
            
  

