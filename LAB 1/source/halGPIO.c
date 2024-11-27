#include  "../header/halGPIO.h"     // private library - HAL layer

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
}
//--------------------------------------------------------------------
// 				Print Byte to 8-bit LEDs array 
//--------------------------------------------------------------------
void print2LEDs(unsigned char ch){
	LEDsArrPort = ch;
}    
//--------------------------------------------------------------------
//				Clear 8-bit LEDs array 
//--------------------------------------------------------------------
void clrLEDs(void){
	LEDsArrPort = 0x000;
}  
//--------------------------------------------------------------------
//				Toggle 8-bit LEDs array 
//--------------------------------------------------------------------
void toggleLEDs(char ch){
	LEDsArrPort ^= ch;
}
//--------------------------------------------------------------------
//				Set 8-bit LEDs array 
//--------------------------------------------------------------------
void setLEDs(char ch){
	LEDsArrPort |= ch;
}
//--------------------------------------------------------------------
//				Read value of 4-bit SWs array 
//--------------------------------------------------------------------
unsigned char readSWs(void){
	unsigned char ch;
	
	ch = PBsArrPort;
	ch &= SWmask;     // mask the least 4-bit
	return ch;
}
//---------------------------------------------------------------------
//             Increment / decrement LEDs shown value 
//---------------------------------------------------------------------
void incLEDs(char val){
	LEDsArrPort += val;
}
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//*********************************************************************
//            Port2 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void PBs_handler(void){
   
              delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
       if(PBsArrIntPend & PB0 || PBsArrIntPend == 0x81){
         
          PBsArrIntPend &= ~PB0;
          state = state1;
        }
        else if(PBsArrIntPend & PB1 || PBsArrIntPend == 0x82){
          
          state = state2;
	  PBsArrIntPend &= ~PB1; 
        }
	else if(PBsArrIntPend & PB2 || PBsArrIntPend == 0x84){ 
	  state = state3;
	  PBsArrIntPend &= ~PB2;
        }
       else if(PBsArrIntPend & PB3 || PBsArrIntPend == 0x88){ 
	  state = state4;
	  PBsArrIntPend &= ~PB3;
        }
//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}
 


//--------------------------------------------------------------------
// 				Print Bit to port2.7
//--------------------------------------------------------------------

void print2_7(int num)
{
    if (num == 1)
    {
        PORT2OUT |= 0x80;
    }
    else
    {
      PORT2OUT &= 0x7F;
    }
}

//--------------------------------------------------------------------
// 				turn off flag
//--------------------------------------------------------------------

void flag_off(int pin)
{
    switch(pin){
		case 1:
		 PBsArrIntPend &= 0xFE;
		 break;
		 
		case 2:
		 PBsArrIntPend &= 0xFD; 
		 break;
		 
		case 3:
		 PBsArrIntPend &= 0x7B;
		 break;
                 
                case 4:
		 PBsArrIntPend &= 0x7F;
		 break;
	}
}






