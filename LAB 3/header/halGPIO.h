#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable
extern unsigned int KB;
extern unsigned int i;
extern unsigned int scroll;
#define RecorderSize        32
extern char idiom_recorder[32];
extern unsigned int flag2;
extern  char twoopc_0 ;
extern  char twoopc_1 ;
extern  char threeopc_0 ;
extern  char threeopc_1 ;
extern  char threeopc_2 ;
char strMerge[102];
extern unsigned int dma0_transferred;
extern unsigned int dma1_transferred;
extern unsigned int KBIFG;
extern unsigned int flag;
extern unsigned int push ;
extern unsigned int two_opc;
extern unsigned int three_opc ;
extern unsigned int preKB;
void print_char();

extern void sysConfig(void);
extern void SetByteToPort(char);
extern void clrPortByte(char);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void write_freq_tmp_LCD();
extern char data_matrix[10][37];
extern void disable_ADC10();
extern void enable_ADC10();
extern void enableDMA1();
extern void setSourceDestDMA1(int source);
extern void setSourceDestDMA0(int source, int dest);
extern void disable_TimerBCCIE();
extern void enable_TimerBCCIE();
extern void timer_B_on();
extern void TAOnEnable();
extern void TAOnDisable();
extern void DMA0_STATE3(int *SRC, int len1, int *DST);
extern void DMA0_STATE2(int *ptr1, int len1, int *ptr_merge);
extern void DMA1_STATE2(int *ptr2, int len2, int *ptr_merge);
extern void write_signal_shape_tmp_LCD();
extern int find_word(char str[]);
extern __interrupt void PBs_handler(void);
extern __interrupt void PBs_handler_P2(void);
extern __interrupt void Timer_A0(void);
extern __interrupt void DMA_ISR(void);
extern __interrupt void Timer_A1(void);
extern __interrupt void Timer_B (void);

#endif

#ifdef CHECKBUSY
    #define LCD_WAIT lcd_check_busy()
#else
    #define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)   (!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)   (!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction

#define LCD_RS(a)   (!a ? (P2OUT&=~0X40) : (P2OUT|=0X40)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)   (!a ? (P2DIR&=~0X40) : (P2DIR|=0X40)) // P2.6 pin direction

#define LCD_RW(a)   (!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)   (!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset


/*---------------------------------------------------------
  END CONFIG
-----------------------------------------------------------*/
#define FOURBIT_MODE    0x0
#define EIGHTBIT_MODE   0x1
#define LCD_MODE        FOURBIT_MODE

#define OUTPUT_PIN      1
#define INPUT_PIN       0
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00

#define LCD_STROBE_READ(value)  LCD_EN(1), \
                asm("nop"), asm("nop"), \
                value=LCD_DATA_READ, \
                LCD_EN(0)

#define lcd_cursor(x)       lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()         lcd_cmd(0x01)
#define lcd_putchar(x)      lcd_data(x)
#define lcd_goto(x)         lcd_cmd(0x80+(x))
#define lcd_cursor_right()  lcd_cmd(0x14)
#define lcd_cursor_left()   lcd_cmd(0x10)
#define lcd_display_shift() lcd_cmd(0x1C)
#define lcd_home()          lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_init();
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);






