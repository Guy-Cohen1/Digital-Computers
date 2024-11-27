#include  "../header/halGPIO.h"

unsigned int cycle = 0;

void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
}

unsigned char readSWs(void) {
    unsigned char ch;
    ch = SWsArrPort;
    ch &= 0x01;
    return ch;
}

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
		LCD_WAIT;
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET;
		lcd_strobe();
		lcd_cmd(0x28);
	} else lcd_cmd(0x3C);
	
	lcd_cmd(0xF);
	lcd_cmd(0x1);
	lcd_cmd(0x6);
	lcd_cmd(0x80);
    enable_interrupts();
}

void lcd_cmd(unsigned char c){
  
	LCD_WAIT;

	if (LCD_MODE == FOURBIT_MODE) {
		LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
    	LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	} else {
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}

void lcd_data(unsigned char c){
        
	LCD_WAIT;

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE) {
    	LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
        LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	} else {
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
          
	LCD_RS(0);   
}

void DelayUs(unsigned int cnt){
	unsigned char i;
    for(i=cnt ; i>0 ; i--) asm("noP");
}

void DelayMs(unsigned int cnt){
	unsigned char i;
    for(i=cnt ; i>0 ; i--) DelayUs(1000);
}

void lcd_strobe(){
  LCD_EN(1);
  asm("nop");
  LCD_EN(0);
}

void delay(unsigned int t){
	volatile unsigned int i;
	for(i=t; i>0; i--);
}

void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);
    else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);
    else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);
    else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);
}

void enable_interrupts(){
  _BIS_SR(GIE);
}

void lcd_puts(const char * s){
	while(*s)
		lcd_data(*s++);
}

void disable_interrupts(){
  _BIC_SR(GIE);
}

void enable_TimerA0() {
    TA0CTL =  MC_3 + TACLR;
}

void disable_TimerA0(){
  TA0CTL &= ~(MC_3 + TAIE);
}

void enable_TimerA1(){
  TA1CTL |= MC_2 + TACLR;
}

void enable_TimerA1Q2(){
  TA1CTL |= MC_1 + TACLR;
  TA1CCTL1 = OUTMOD_7;
}

void disable_TimerA1(){
  TA1CTL &= ~MC_3;
  TA1CCTL1 = OUTMOD_0;
}

void enable_TA1CCTL2(){
  TA1CTL |= TAIE;
  TA1CCTL2 |= CCIE;
  TA1CCTL2 &= ~(CCIFG);
}

void enable_ADC10(){
  ADC10CTL0 |= ENC + ADC10SC;
}

void disable_ADC10(){
  
}

void writeFreqLCD(){
  lcd_clear();
  lcd_home();
  const char fin[] = "fin=";
  const char hz[] = "Hz";
  lcd_puts(fin);
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_puts(hz);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (TA1IV) {
        case 0x2:
            break;
        case 0x4:
            if (cycle == 0) {
                cycle = 1;
                start = TA1CCR2;
            } else {
                cycle = 0;
                end = TA1CCR2;
                TA1CCTL2 &= ~CCIE;
                __bic_SR_register_on_exit(LPM0_bits + GIE);
            }
            break;
        case 0xa:
            break;
        default:
            break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(CPUOFF);
}

#pragma vector=PORT1_VECTOR
__interrupt void PBs_handler(void){
      
	delay(debounceVal);

	if(PBsArrIntPend & PB0){
	  state = state1;
	  PBsArrIntPend &= ~PB0;
    } else if(PBsArrIntPend & PB1){
	  state = state2;
	  PBsArrIntPend &= ~PB1; 
    } else if(PBsArrIntPend & PB2){ 
	  state = state3;
	  PBsArrIntPend &= ~PB2;
    } else if(P2IFG & 0x02){
	  state = state4;
	  P2IFG &= ~0x02;
    }

    switch(lpm_mode){
		case mode0:
		 LPM0_EXIT;
		 break;
		case mode1:
		 LPM1_EXIT;
		 break;
		case mode2:
		 LPM2_EXIT;
		 break;
        case mode3:
		 LPM3_EXIT;
		 break;
        case mode4:
		 LPM4_EXIT;
		 break;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void PBs_handler2(void){
      
	delay(debounceVal);

    if(P2IFG & 0x02){
	  state = state4;
	  P2IFG &= ~0x02;
    }

    switch(lpm_mode){
		case mode0:
		 LPM0_EXIT;
		 break;
		case mode1:
		 LPM1_EXIT;
		 break;
		case mode2:
		 LPM2_EXIT;
		 break;
        case mode3:
		 LPM3_EXIT;
		 break;
        case mode4:
		 LPM4_EXIT;
		 break;
	}
}


