#ifndef _halGPIO_H_
#define _halGPIO_H_
#define MULTIPLY_FP_RESOLUTION_BITS 15
#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer
#include "stdint.h"

#define MULTIPLY_FP_RESOLUTION_BITS 15 // For atan2-fixed point

extern char arr_print[];
extern char arr_print_temp[];
extern enum FSMstate state;   // global variable
extern enum Stepperstate stateStepp;
extern enum SYSmode lpm_mode; // global variable
extern unsigned int delay_time;
extern int startRotateLEDs;
extern int* rotateLEDs;
extern int pushedIFG;
extern int sendIFG;
extern short FIRSTBYTE_FLAG;
extern short paint_flag;
extern const short PB_PAINT_str[];
extern char RxData[];
extern char Rec_data[];
extern int ExecuteFlag;
extern int transfer_to_flash;
extern int SendFlag;
extern int j;
extern short Vx_Vy[];
extern int counter;
extern int rotateIFG;
extern char last_samples[];
extern unsigned int i;
extern unsigned int TX_counter;
#define half_sec 500;
#define clk_tmp 131; // ( (2^20) / 8 )*(10^-3) to convert ms to counter value for TACCR0
extern int curr_counter;

extern void sysConfig(void);
extern void SetByteToPort(char);
extern void clrPortByte(char);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();

// LCD
extern void lcd_puts(const char * s);
extern void lcd_strobe();
extern void lcd_data(unsigned char);
extern void lcd_cmd(unsigned char);
extern void lcd_init();

extern void convert_to_str(int num);
extern void disable_interrupts();
extern void timer_call_counter();
extern void TIMERA0_delay_ms(unsigned int counter);
extern void START_TIMERA1(unsigned int counter);
extern float angle(float X, float Y);
extern void  move_pointer_to_specific_deg(uint32_t stepper_degrees, char script_state);
extern int16_t tangens(int16_t y_fp, int16_t x_fp);
extern void ACK_to_PC();
extern uint32_t convert_hex_to_int(char *hex);

extern __interrupt void Timer_A0(void);
extern __interrupt void Timer_A1(void);
extern __interrupt void Timer1_A0_ISR(void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void USCI0TX_ISR(void);
extern __interrupt void Joystick_handler(void);

#endif

// #define CHECKBUSY    1  // using this define, only if we want to read from LCD

#ifdef CHECKBUSY
    #define LCD_WAIT lcd_check_busy()
#else
    #define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)   (!a ? (P1OUT&=~0X01) : (P1OUT|=0X01)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)   (!a ? (P1DIR&=~0X01) : (P1DIR|=0X01)) // P2.5 pin direction

#define LCD_RS(a)   (!a ? (P1OUT&=~0X40) : (P1OUT|=0X40)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)   (!a ? (P1DIR&=~0X40) : (P1DIR|=0X40)) // P2.6 pin direction

#define LCD_RW(a)   (!a ? (P1OUT&=~0X80) : (P1OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)   (!a ? (P1DIR&=~0X80) : (P1DIR|=0X80)) // P2.7 pin direction

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

/*
 *  Delay functions for HI-TECH C on the PIC18
 *
 *  Functions available:
 *      DelayUs(x)  Delay specified number of microseconds
 *      DelayMs(x)  Delay specified number of milliseconds
*/






