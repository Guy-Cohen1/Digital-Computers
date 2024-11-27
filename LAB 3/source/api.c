#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


//-------------------------------------------------------------
//                     Merge - Task 2
//-------------------------------------------------------------
void Merge() {
    char merge1[38], merge2[38];
    char strMerge[76];
    int len1=0, len2=0, sent1len=0, sent2len=0;
    char *ptr1, *ptr2, *ptr_merge, *print;
    char *end1, *end2;
    char *space_pos;
    int lcdptr = 0;
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    StopAllTimers();

    while(state==state2){
        lcd_home();
        if(state != state2)
            return;
        lcd_puts("First index: ");
        lcd_goto(0x40);
        delay(1500);
        delay(1500);
        delay(1500);
        delay(1500);
        if(state != state2)
                    return;
        __bis_SR_register(LPM0_bits + GIE);
        if(KB==16)KB=0;
        strcpy(merge1, data_matrix[KB]);
        if(state != state2)
                    return;
        lcd_clear();
        lcd_home();
        if(state != state2)
                    return;
        lcd_puts("Second index: ");
        if(state != state2)
                    return;
        lcd_goto(0x40);
        delay(1500); // debounce
        delay(1500);
        delay(1500);
        delay(1500);
        if(state != state2)
                    return;
        __bis_SR_register(LPM0_bits + GIE);
        if(state != state2)
                    return;
        if(KB==16)KB=0;
        strcpy(merge2, data_matrix[KB]);
        if(state != state2)
                    return;
        lcd_clear();
        lcd_home();

        ptr1 = merge1;
        ptr2 = merge2;
        ptr_merge = strMerge;
        sent1len = strlen(merge1);
        sent2len = strlen(merge2);
        end1 = merge1 + sent1len;
        end2 = merge2 + sent2len;
        strMerge[0] = '\0'; // init the string
        DMACTL0 = DMA0TSEL_0;  // No trigger source (manual transfer)

        while (ptr1 < end1 || ptr2 < end2 && state == state2) {
            // Find the next word in merge1
            if(state != state2)
                        return;
            if (ptr1 < end1) {
                space_pos = strchr(ptr1, ' ');
                if (space_pos) {
                    len1 = space_pos - ptr1;
                } else { // space not found (last word in the sentence)
                    len1 = end1 - ptr1;
                }
                DMA0_STATE2(ptr1, len1, ptr_merge);  // Use DMA to transfer the word

                ptr1 += len1;
                ptr_merge += len1;

                if (*ptr1 == ' ') {
                    ptr1++;
                    *ptr_merge = ' ';
                    ptr_merge++;
                } else if (ptr1 < end1 || ptr2 < end2) {
                    *ptr_merge = ' ';
                    ptr_merge++;
                }
            }

            // Find the next word in merge2
            if (ptr2 < end2) {
                if(state != state2)
                            return;
                space_pos = strchr(ptr2, ' ');
                if (space_pos) {
                    len2 = space_pos - ptr2;
                } else {
                    len2 = end2 - ptr2;
                }
                DMA1_STATE2(ptr2, len2, ptr_merge);  // Use DMA to transfer the word

                ptr2 += len2;
                ptr_merge += len2;

                if (*ptr2 == ' ') {
                    ptr2++;
                    *ptr_merge = ' ';
                    ptr_merge++;
                } else if (ptr1 < end1 || ptr2 < end2) {
                    *ptr_merge = ' ';
                    ptr_merge++;
                }
            }
        }
        // Remove trailing space
        if (ptr_merge > strMerge && *(ptr_merge - 1) == ' ') {
            if(state != state2)
                        return;
            *(ptr_merge - 1) = '\0';
        } else {
            *ptr_merge = '\0';
        }
        if(state != state2)
                    return;
        lcd_clear();
        lcd_home();
        print = strMerge;
        while(*print != '\0'){
            if(state != state2)
                        return;
            lcd_data(*print);
            print++;
            lcdptr++;
            if(lcdptr==16){
                lcd_new_line;
            }
            else if(lcdptr==32){
                lcd_home();
                lcdptr = 0;
                __bis_SR_register(LPM0_bits + GIE);
                if(state != state2)
                            return;
                lcd_clear();
            }
        }
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    }
}
// Call LCD reset for printing which song we playing
void lcd_reset(){
    lcd_clear();
    lcd_home();
}

//******************************************************************
// state3 functions
//******************************************************************
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void printArrayToLED() {
    int numbers[] = {128, 64, 32, 16, 8, 4, 23, 13, 40};
    while (state == state3) {
        int i;
        for (i = 0; i < 8; i++) {
            TIMERconfig();
            disable_TimerBCCIE();
            if(state == state3)
                DMA0_STATE3(&numbers[i], 1, &LEDsArrPort);
            else
                return;
            __delay_cycles(100000);
        }
    }
    disable_TimerBCCIE();

}

