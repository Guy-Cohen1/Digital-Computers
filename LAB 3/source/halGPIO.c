#include  "../header/halGPIO.h"     // private library - HAL layer

// Global Variables
unsigned int scroll = 0;
int j=0;
unsigned int KBIFG = 30;
extern unsigned int dma0_transferred =0;
extern unsigned int dma1_transferred =0;
extern unsigned int flag =0;
extern unsigned int flag2 =0;
extern unsigned int push =0;
extern unsigned int two_opc=0;
extern unsigned int three_opc =0;
extern unsigned int preKB=0;
extern unsigned int flag_kb=0;
char idiom_recorder[32];
extern  char twoopc_0 = 0;
extern  char twoopc_1 = 0;
extern  char threeopc_0 = 0;
 char strMerge[102];
extern  char threeopc_1 = 0;
extern  char threeopc_2=0  ;
extern char data_matrix[10][37] = {
    "An apple a day keeps the doctor away",
    "climb on the bandwagon",
    "Dot the i's and cross the t's",
    "He who pays the piper calls the tune",
    "The pen is mightier than the sword",
    "The pot calling the kettle black",
    "shed crocodile tears",
    "Close but no cigar",
    "Cut from the same cloth",
    "Strike while the iron’s hot"
};




//----------------------------------------------------------------------------------------------
void DMA0_STATE2(int *ptr1, int len1, int *ptr_merge){
     // Use DMA to transfer the word
    DMA0SA = (void (*)())ptr1;  // Source address for merge1
    DMA0DA = (void (*)())ptr_merge;  // Destination address for strMerge
    DMA0SZ = len1;  // Block size
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1 + DMASRCBYTE + DMADSTBYTE;  // Enable DMA, source and destination increment, block transfer mode
    DMA0CTL |= DMAREQ;  // Manually trigger DMA transfer for merge1 word
    while (DMA0CTL & DMAEN);  // Wait for DMA transfer to complete
}

void DMA1_STATE2(int *ptr2, int len2, int *ptr_merge){
    // Use DMA to transfer the word
    DMA1SA = (void (*)())ptr2;  // Source address for merge2
    DMA1DA = (void (*)())ptr_merge;  // Destination address for strMerge
    DMA1SZ = len2;  // Block size
    DMA1CTL = DMAEN + DMASRCINCR_3 + DMADSTINCR_3 + DMADT_1 + DMASRCBYTE + DMADSTBYTE;  // Enable DMA, source and destination increment, block transfer mode
    DMA1CTL |= DMAREQ;  // Manually trigger DMA transfer for merge2 word
    while (DMA1CTL & DMAEN);  // Wait for DMA transfer to complete
}
//---------------------------------------------------------------
void DMA0_STATE3(int *SRC, int len1, int *DST){
     // Use DMA to transfer the word
    DMACTL0 = DMA0TSEL_8;
    DMA0SA = (void (*)())SRC;  // Source address for merge1
    DMA0DA = &LEDsArrPort;  // Destination address for strMerge
    DMA0SZ = len1;  // Block size
    DMA0CTL = DMAEN + DMASRCINCR_3 + DMADT_1 + DMASBDB + DMAIE;  // Enable DMA, source and destination increment, block transfer mode
    while (DMA0CTL & DMAEN);  // Wait for DMA transfer to complete
}
//-------------------------------------------------------------------------------------------------
void print_char()
{
    if(KB != preKB)
    {
        if (flag_kb ==1)
        {
            lcd_cursor_left();
            flag_kb = 0;
            two_opc = 0;
            three_opc = 0;
            push = 0;
        }

        two_opc = 0;
        three_opc = 0;
        push = 0;
        if(i != 0 )
        {
            lcd_cursor_right();
        }
        i++;
        if(KB < 10)
        {
            lcd_data(0x30 + KB);
            idiom_recorder[i-1] = KB;
        }
        else if(KB == 15)
        {
            lcd_data('A');
            idiom_recorder[i-1] = 'A';
        }
        else if(KB == 14)
        {
            lcd_data('B');
            idiom_recorder[i-1] = 'B';
        }
        else if(KB == 13)
        {
            lcd_data('F');
            idiom_recorder[i-1] = 'F';
        }
        else if(KB == 12)
        {
            lcd_data('E');
            idiom_recorder[i-1] = 'E';
        }
        else if(KB == 11)
        {
            lcd_data('D');
            idiom_recorder[i-1] = 'D';
        }
        else if(KB == 10)
        {
            lcd_data('C');
            idiom_recorder[i-1] = 'C';
        }
        if (i != 0 )
            lcd_cursor_left();
        if(i != 0)
            disable_interrupts();
    }
    else if(KB == preKB)
    {

        if(KB != 15 && KB != 14 && KB != 13 && KB != 0)
        {
            if(two_opc == 0)
            {

                if( KB == 1 )
                {
                    lcd_data(0x30 + 1);
                    twoopc_0 = 1;
                    idiom_recorder[i-1] = 1;
                }
                else if( KB == 2 )
                {
                    lcd_data(0x30 + 2);
                    idiom_recorder[i-1] = 2;
                }
                else if( KB == 3 )
                {
                    lcd_data(0x30 + 3);
                    twoopc_0 = 3;
                    idiom_recorder[i-1] = 3;
                }
                else if( KB == 4 )
                {
                    lcd_data(0x30 + 4);
                    twoopc_0 = 4;
                    idiom_recorder[i-1] = 4;
                }
                else if( KB == 5 )
                {
                    lcd_data(0x30 + 5);
                    twoopc_0 = 5;
                    idiom_recorder[i-1] = 5;
                }
                else if( KB == 6 )
                {
                    lcd_data(0x30 + 6);
                    twoopc_0 = 6;
                    idiom_recorder[i-1] = 6;
                }
                else if( KB == 7)
                {
                    lcd_data(0x30 + 7);
                    twoopc_0 = 7;
                    idiom_recorder[i-1] = 7;
                }
                else if( KB == 8 )
                {
                    lcd_data(0x30 + 8);
                    twoopc_0 = 8;
                    idiom_recorder[i-1] = 8;
                }
                else if( KB == 9 )
                {
                    lcd_data(0x30 + 9);
                    twoopc_0 = 9;
                    idiom_recorder[i-1] = 9;
                }

                else if(KB == 12)
                {
                    lcd_data('E');
                    twoopc_0 = 'E';
                    idiom_recorder[i-1] = 'E';
                }
                else if(KB == 11)
                {
                    lcd_data('D');
                    twoopc_0 = 'D';
                    idiom_recorder[i-1] = 'D';
                }
                else if(KB == 10)
                {
                   lcd_data('C');
                   twoopc_0 = 'C';
                   idiom_recorder[i-1] = 'C';
                }
            }
            else if(two_opc == 1)
            {
                if(KB == 1)
                {
                    lcd_data('G');
                    twoopc_1 = 'G';
                    idiom_recorder[i-1] = 'G';
                }
                else if(2 == KB)
                {
                   lcd_data('H');
                   twoopc_1 = 'H';
                   idiom_recorder[i-1] = 'H';
                }
                else if( KB == 3 )
                {
                    lcd_data('I');
                    twoopc_1 = 'I';
                    idiom_recorder[i-1] = 'I';
                }

                else if(4 == KB)
                {
                   lcd_data('K');
                   twoopc_1 = 'K';
                   idiom_recorder[i-1] = 'K';
                }
                else if(5 == KB)
                {
                   lcd_data('L');
                   twoopc_1 = 'L';
                   idiom_recorder[i-1] = 'L';
                }
                else if(6 == KB)
                {
                   lcd_data('M');
                   twoopc_1 = 'M';
                   idiom_recorder[i-1] = 'M';
                }
                else if(7 == KB)
                {
                   lcd_data('O');
                   twoopc_1 = 'O';
                   idiom_recorder[i-1] = 'O';
                }
                else if(8 == KB)
                {
                   lcd_data('P');
                   twoopc_1 = 'P';
                   idiom_recorder[i-1] = 'P';
                }
                else if(9 == KB)
                {
                   lcd_data('Q');
                   twoopc_1 = 'Q';
                   idiom_recorder[i-1] = 'Q';
                }


                else if(KB == 12)
                {
                    lcd_data('R');
                    twoopc_1 = 'R';
                    idiom_recorder[i-1] = 'R';
                }
                else if(KB == 11)
                {
                    lcd_data('N');
                    twoopc_1 = 'N';
                    idiom_recorder[i-1] = 'N';
                }
                else if(KB == 10)
                {
                    lcd_data('J');
                    twoopc_1 = 'J';
                    idiom_recorder[i-1] = 'J';
                }
            }
            push++;
            two_opc = push % 2;
            three_opc = push % 3;
        }
        else
        {
            if(three_opc == 0)
            {
                if(KB == 0)
                {
                    lcd_data(0x30);
                    threeopc_0 = 0;
                    idiom_recorder[i-1] = 0;
                }
                else if(KB == 15)
                {
                    lcd_data('A');
                    threeopc_0 = 'A';
                    idiom_recorder[i-1] = 'A';
                }
                else if(KB == 14)
                {
                    lcd_data('B');
                    threeopc_0 = 'B';
                    idiom_recorder[i-1] = 'B';
                }
                else if(KB == 13)
                {
                    lcd_data('F');
                    threeopc_0 = 'F';
                    idiom_recorder[i-1] = 'F';
                }
            }
            else if(three_opc == 1)
            {
                if(KB == 0)
                {
                    lcd_data('U');
                    threeopc_1 = 'U';
                    idiom_recorder[i-1] = 'U';
                }
                else if(KB == 15)
                {
                    lcd_data('S');
                    threeopc_1 = 'S';
                    idiom_recorder[i-1] = 'S';
                }
                else if(KB == 14)
                {
                    lcd_data('W');
                    threeopc_1 = 'W';
                    idiom_recorder[i-1] = 'W';
                }
                else if(KB == 13)
                {
                    lcd_data('Y');
                    threeopc_1 = 'Y';
                    idiom_recorder[i-1] = 'Y';
                }
            }
            else if(three_opc == 2)
            {
                if(KB == 0)
                {
                    lcd_data('V');
                    threeopc_2 = 'V';
                    idiom_recorder[i-1] = 'V';
                }
                else if(KB == 15)
                {
                    lcd_data('T');
                    threeopc_2 = 'T';
                    idiom_recorder[i-1] = 'T';
                }
                else if(KB == 14)
                {
                    lcd_data('X');
                    threeopc_2 = 'X';
                    idiom_recorder[i-1] = 'X';
                }
                else if(KB == 13)
                {
                    lcd_data('Z');
                    threeopc_2 = 'Z';
                    idiom_recorder[i-1] = 'Z';
                }
            }
           push++;
           two_opc = push % 2;
           three_opc = push % 3;

            }
            lcd_cursor_left();
            if (i ==0)
                i++;
            }
            if(i == 16 && flag2 == 0)
            {
                lcd_new_line;
                lcd_cursor_left();
                flag2 = 1;
            }
}
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
    //StopAllTimers();
    GPIOconfig();
    //TIMERconfig();
	lcd_init();
	lcd_clear();
}
//--------------------------------------------------------------------
// 				Set Byte to Port
//--------------------------------------------------------------------
void SetByteToPort(char ch){
	PBsArrPortOut |= ch;  
} 
//--------------------------------------------------------------------
// 				Clear Port Byte
//--------------------------------------------------------------------
void clrPortByte(char ch){
	PBsArrPortOut &= ~ch;
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
//---------------------------------------------------------------------
//            LCD
//---------------------------------------------------------------------
//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    if (LCD_MODE == FOURBIT_MODE)
    {
        LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
        lcd_strobe();
    }
    else
    {
        LCD_DATA_WRITE = c;
        lcd_strobe();
    }
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_RS(1);
    if (LCD_MODE == FOURBIT_MODE)
    {
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
            LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
    }
    else
    {
            LCD_DATA_WRITE = c;
            lcd_strobe();
    }

    LCD_RS(0);
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){

    char init_value;

    if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
    else init_value = 0x3F;

    LCD_RS_DIR(OUTPUT_PIN);
    LCD_EN_DIR(OUTPUT_PIN);
    LCD_RW_DIR(OUTPUT_PIN);
    LCD_DATA_DIR |= OUTPUT_DATA;
    LCD_RS(0);
    LCD_EN(0);
    LCD_RW(0);

    DelayMs(15);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayMs(5);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayUs(200);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();

    if (LCD_MODE == FOURBIT_MODE){
        LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
        lcd_strobe();
        lcd_cmd(0x28); // Function Set
    }
    else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots

    lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
    lcd_cmd(0x1); //Display Clear
    lcd_cmd(0x6); //Entry Mode
    lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm("NOP");
 // asm("NOP");
  LCD_EN(0);
}
//---------------------------------------------------------------------
//                     Polling delays
//---------------------------------------------------------------------
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec

}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec

}
//******************************************************************
//            Polling based Delay function
//******************************************************************
void delay(unsigned int t){  //
    volatile unsigned int i;

    for(i=t; i>0; i--);
}
//---------------**************************----------------------------
//               Interrupt Services Routines
//---------------**************************----------------------------

//*********************************************************************
  //            TimerB Interrupt Service Rotine
  //*********************************************************************
  #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
  #pragma vector=TIMERB0_VECTOR//TIMERB0_VECTOR||TIMER0_B0_VECTOR
  __interrupt void Timer_B (void)
  #elif defined(__GNUC__)
  void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
  #else
  #error Compiler not supported!
  #endif
  {
    LPM0_EXIT;
  }
//*********************************************************************
//            Port1 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT1_VECTOR  // For Push Buttons
  __interrupt void PBs_handler(void){
	delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	if(PBsArrIntPend & PB0){

	    state = state1;
	    DMACTL0 = DMA0TSEL_0;
	    DMA0CTL &= ~DMAEN;
	    lcd_clear();
	    LEDsArrPort = 0X00;
        lcd_home();
        lcd_puts("Enter a sentence: ");
	    PBsArrIntPend &= ~PB0;
	}
	// If PB1, Show LCD Menu and reset scroll
    else if(PBsArrIntPend & PB1){
        state = state2;
        DMA0CTL &= ~DMAEN;
        DMACTL0 = DMA0TSEL_0;
        lcd_clear();
        LEDsArrPort = 0X00;
        lcd_home();
        flag_kb = 0;
	    PBsArrIntPend &= ~PB1;
	    disable_TimerBCCIE();

    }
	// If PB2, Show LCD Menu according the scroll value
    else if(PBsArrIntPend & PB2){
            DMACTL0 = DMA0TSEL_0;
            DMA0CTL &= ~DMAEN;
            state = state3;
            disable_TimerBCCIE();
            lcd_clear();



	    PBsArrIntPend &= ~PB2;
        }
    else if(PBsArrIntPend & PB3){

            DMACTL0 = DMA0TSEL_0;
            state = state4;
            lcd_clear();

            PBsArrIntPend &= ~PB3;
        }
	     StopAllTimers();
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
//*********************************************************************
//            Port2 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT2_VECTOR  // For KeyPad
  __interrupt void PBs_handler_P2(void){
      delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
      if(KeypadIRQIntPend & BIT1){    // if keypad has been pressed find value
          preKB = KB;
          KB = 75;
          KeypadPortOUT = 0x0E;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 13;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 14;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 0;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 15; //EndOfRecord = 1; } // We chose (*) to stop recording

          KeypadPortOUT = 0x0D;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 12;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 9;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 8;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 7;

          KeypadPortOUT = 0x0B;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 11;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 6;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 5;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 4;

          KeypadPortOUT = 0x07;
          if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 10;
          else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 3;
          else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 2;
          else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 1;
//----------------Statement1--------------------------------------
          if (state == state1){
              if (KB != 75) {
                  TIMER_A0_config();
                  TA0R = 0X0000;
                  if(i ==0)
                  {
                      lcd_clear();
                      lcd_home();
                  }
                  print_char();
              }

          }
          delay(15000);   // For keypad debounce
          KeypadPortOUT &= ~0x0F;  // Reset Row1-4
          KeypadIRQIntPend &= ~BIT1; // Reset Flag
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
//*********************************************************************
//            TIMER A0 ISR
//*********************************************************************
#pragma vector = TIMERA0_VECTOR // For delay
__interrupt void TimerA_ISR (void)
{
    if (state == state1){
            lcd_cursor_right();
            i++;
            disable_interrupts();
            push = 0;
            if (two_opc == 0 )
                idiom_recorder[i] = twoopc_0;
            else if (two_opc == 1 )
                idiom_recorder[i] = twoopc_1;
            else if (three_opc == 0 )
                idiom_recorder[i] = threeopc_0;
            else if (three_opc == 1 )
                idiom_recorder[i] = threeopc_1;
            else if (three_opc == 2 )
                idiom_recorder[i] = threeopc_2;
            two_opc = 0;
            three_opc = 0;
            flag_kb = 1;
            StopAllTimers();
    }
}
//*********************************************************************
//            DMA interrupt service routine
//*********************************************************************
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
{
 DMA0CTL &= ~DMAIFG;
 DMA1CTL &= ~DMAIFG;
 __bic_SR_register_on_exit(LPM0_bits);       // Exit LPMx, interrupts enabled
}
//---------------------------------------------------------------------
//            Activate timer B in up mode
//---------------------------------------------------------------------
void timer_B_on(){
    TB0R = 0x00;
    TBCTL |= MC_1;
    TB0CCR0 = 0xFFFF; // Set TB0CCR0 to the maximum value
    __bis_SR_register(LPM0_bits);     // Enter Low Power Mode 0
}
//---------------------------------------------------------------------
//            Enable TimerB ccie
//---------------------------------------------------------------------
void enable_TimerBCCIE(){
        TB0CCTL0 |= CCIE;
}
//---------------------------------------------------------------------
//            Disable TimerB ccie
//---------------------------------------------------------------------
void disable_TimerBCCIE(){
    TB0CCTL0 &= ~(CCIE);
}
//******************************************************************
//      DMA routines
//******************************************************************
//---------------------------------------------------------------------
//            setSourceDestDMA0
//---------------------------------------------------------------------
void setSourceDestDMA0(int source, int dest){
   DMA0SA = source;
   DMA0DA = dest;
}
//---------------------------------------------------------------------
//            setSourceDestDMA1
//---------------------------------------------------------------------
void setSourceDestDMA1(int source){
   DMA1SA = source;
}
//---------------------------------------------------------------------
//            trigerDMA1
//---------------------------------------------------------------------
void enableDMA1(){
    DMA1CTL |= DMAEN;
}


