#include "../header/api.h"
#include "../header/halGPIO.h"
#include <string.h>
unsigned int start;
unsigned int end;
int count;
char clcFreq[6];
#define ADC_NUMBER_CAPTURES 100
unsigned int adcCaptureValues[ADC_NUMBER_CAPTURES];
unsigned int adcCapturePointer;

void findHz() {
    float number;
    float freq;
    static float prev_number = 0;

    writeFreqLCD();
    enable_TimerA1();

    while (state == state1) {
        disable_interrupts();
        clcFreq[5] = '\0';

        start = end = 0;

        enable_TA1CCTL2();
        __bis_SR_register(LPM0_bits + GIE);
        asm("nop");

        if ((start == 0) && (end == 0))
            continue;

        number = end - start;

        if ((number == prev_number + 1) || (number == prev_number - 1) || (number == prev_number - 2) || (number == prev_number + 2)) {
            number = prev_number;
        } else {
            prev_number = number;
        }

        freq = 1048576 / number;
        
        float mult_factor = getMultiplicativeFactor(freq);
        freq *= mult_factor;
        int intFreq = (int)(freq);
        if ((freq - intFreq) >= 0.5) {
            freq = intFreq + 1;
        } else {
            freq = intFreq;
        }

        NumToString((int)freq);

        lcd_home();
        lcd_cursor_right();
        lcd_cursor_right();
        lcd_cursor_right();
        lcd_cursor_right();

        if (clcFreq[4] != ' ')
            lcd_puts(clcFreq);

        cursor_off;
        DelayMs(500);
        enable_interrupts();
    }

    disable_TimerA1();
}

void NumToString(int num) {
    int num2 = num;
    int counter = 0;

    while (num2 != 0) {
        num2 = num2 / 10;
        counter++;
    }

    int five = 5;

    while (five > 0) {
        if (counter > 0) {
            clcFreq[five - 1] = (num % 10 + '0');
            num = num / 10;
            counter--;
            five--;
        } else {
            clcFreq[five - 1] = ' ';
            five--;
        }
    }
}

float getMultiplicativeFactor(float freq) {
    if (freq < 116) {
        return 1.111;
    } else if (freq < 600) {
        return 1.1188;
    } else if (freq < 1000) {
        return 1.116;
    } else if (freq < 3000) {
        return 1.117;
    } else if (freq < 5000) {
        return 1.116;
    } else if (freq < 8000) {
      return 1.11586;
    } else if (freq < 10000) {
        return 1.119317; 
    } else if (freq < 12001) {
        return 1.110083;
    } else if (freq < 15000) {
        return 1.1158223;
    } else if (freq <= 20000) {
        return 1.108417;
    } else {
        return 1;
    }
}

void UpPauseCount() {
    lcd_clear();
    int sec = 0;
    int running = 0;
    while (state == state2) {
       unsigned char switches = readSWs();

        switch (switches) {
            case 0:
                if (sec == 0) {
                    time_to_lcd(sec);
                }
                while (readSWs() == 0 && state == state2) {}
                break;

            case 1:
                if (!running) {
                    running = 1;
                    lcd_clear();
                }

                for ( ; sec <= 3599 && state == state2; sec++) {                  
                    timerA0On();
                    if (readSWs() == 0) {
                        break;   
                    }
                    time_to_lcd(sec);
                }

                if (sec > 3599) {
                    sec = 0;
                }

                break;

            default:
                break;
        }
    }
}

void timerA0On() {
    TA0R = 0x00;
    TACTL |= MC_3;
    TA0CCR0 = 0xFFFF;
    __bis_SR_register(LPM0_bits);
}

void time_to_lcd(int sec) {
   lcd_home();
   lcd_data(0x30 + ((sec / 60) / 10));
   lcd_data(0x30 + ((sec / 60) % 10));
   lcd_data(':');
   lcd_data(0x30 + ((sec % 60) / 10));
   lcd_data(0x30 + ((sec % 60) % 10));
   cursor_off;
}

void buzzer_gen() {
    enable_TimerA1Q2();
    unsigned int prev_adc_conv = 10000;
    while (state == state3) {
        unsigned int adc_conv = 0;
        enable_ADC10();
        __bis_SR_register(LPM0_bits + GIE);
        adc_conv = ADC10MEM;
        if (adc_conv != prev_adc_conv) {
            prev_adc_conv = adc_conv;
            ADC10CTL0 &= ~ADC10ON;
            float m = 1.466;
            int n = 1000;
            float f_out = m * adc_conv;
            float SMCLK_FREQ = 1048576;
            unsigned int period_to_pwm = (SMCLK_FREQ) / f_out + 48;

            TA1CCR0 = period_to_pwm;
            TA1CCR1 = (int) (period_to_pwm / 2);
        }
    }

    disable_TimerA1();
}
