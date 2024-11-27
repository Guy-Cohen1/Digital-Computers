#include  "../header/api.h"    // private library - API layer
#include  "../header/halGPIO.h" // private library - HAL layer

// Global variables
unsigned int start;
unsigned int end;
int count;
char pot_buffer[16]; // Buffer for converting numbers to strings for display
unsigned int tones[7] = {1001, 801, 667, 571, 500, 444, 400}; // Array of tone frequencies for the buzzer


//---------------------------------------------------------------------
//            RGBBLINK
//---------------------------------------------------------------------
// Function to blink an RGB LED in a sequence
void RGBBLINK() {
    int count = 4;
    while (state == state1) {
        RGB_print(count); // Output the current count to the RGB LED
        if (count == 7)
            count = 0;
        else
            count++;
        timerA0On(); // Delay for x milliseconds
    }
    RGB_print(0); // Turn off the RGB LED
}

//---------------------------------------------------------------------
//            STOPER
//---------------------------------------------------------------------
// Function to act as a stopwatch, displaying the count on the LCD
void STOPER() {
    lcd_clear(); // Clear the LCD
    lcd_home(); // Set the cursor to the home position
    cursor_off; // Turn off the cursor
    while (state == state2) {
        cursor_off; // Ensure cursor is off
        lcd_clear(); // Clear the LCD
        lcd_home(); // Set the cursor to the home position
        intNumToString(countUp); // Convert countUp to a string
        lcd_puts(pot_buffer); // Display the string on the LCD
        countUp++; // Increment the counter
        lcd_home(); // Set the cursor to the home position
        timerA0On(); // Delay for x milliseconds
    }
}

//---------------------------------------------------------------------
//            buzzer series gen
//---------------------------------------------------------------------
// Function to generate a series of tones using the buzzer
void buzzer() {
    unsigned int buz_pos = 0; // Initialize buzzer position to the first tone
    enable_TimerA1_buz(); // Enable TimerA1 in up mode to start the buzzer

    while (state == state3) { // Loop while the system is in state3
        TA1CCR0 = tones[buz_pos]; // Set the period of the timer to the current tone frequency
        TA1CCR2 = TA1CCR0 >> 1; // Set the duty cycle to 50% (TA1CCR2 is half of TA1CCR0)

        timerA0On(); // Call a delay function to wait for the duration specified by x

        if (buz_pos == 6) // If the buzzer position is at the last tone in the array
            buz_pos = 0; // Reset to the first tone
        else
            buz_pos++; // Move to the next tone
    }
    disable_TimerA1(); // Disable TimerA1 when exiting state3
}

//---------------------------------------------------------------------
//            set x
//---------------------------------------------------------------------
// Function to set the value of x
void set_X() {
    cur_state = 0; // Set cur_state to 0 to indicate that x needs to be set
}
// Function to convert an integer to a string and store it in counterToPrint
void intNumToString(int num) {
    int num2 = num;
    int counter = 0;
    while (num2 != 0) {
        num2 = num2 / 10;
        counter++;
    }
    while (counter > 0) {
        pot_buffer[counter - 1] = (num % 10 + '0');
        num = num / 10;
        counter--;
    }
}

//---------------------------------------------------------------------
//            readPOT
//---------------------------------------------------------------------
// Function to read the value of a potentiometer and display it on the LCD
void PRINT_POT() {
    unsigned int vlevel;
    int v_out;
    int flag = 0;
    lcd_clear(); // Clear the LCD
    lcd_home(); // Set the cursor to the home position
    while (state == state5) {
        enable_ADC10(); // Enable ADC10 to start conversion
        __bis_SR_register(LPM0_bits + GIE); // Enter low power mode with interrupts enabled
        vlevel = ADC10MEM; // Read the ADC value
        ADC10CTL0 &= ~ADC10ON; // Turn off ADC10
        v_out = vlevel << 5; // Convert the ADC value to a voltage
        if (v_out < 10000) {
            flag = 1; // Flag to indicate the voltage is less than 1V
        }
        intNumToString(v_out); // Convert the voltage to a string
        int i, index = 0;
        for (i = 0; i < 5; i++) {
            cursor_off; // Turn off the cursor
            if (i == 0) {
                if (flag)
                    lcd_data('0'); // Display '0' if the voltage is less than 1V
                else {
                    lcd_data(pot_buffer[index]); // Display the next digit
                    index++;
                }
            } else if (i == 1)
                lcd_data('.'); // Display the decimal point
            else {
                lcd_data(pot_buffer[index]); // Display the next digit
                index++;
            }
        }
        lcd_home(); // Set the cursor to the home position
        flag = 0; // Reset the flag
    }
    int i;
    for (i = 0; i < 16; i++)
        pot_buffer[i] = 0; // Clear the counterToPrint buffer
}

//---------------------------------------------------------------------
//            reset X and Clr LCD
//---------------------------------------------------------------------
// Function to reset x and clear the LCD
void LCD_CLR_AND_RESET_X() {
    int i = 0;
    for (i = 0; i < 16; i++)
        pot_buffer[i] = 0; // Clear the counterToPrint buffer
    x = 500; // Reset x to 500
    countUp = 0; // Reset countUp to 0
    lcd_clear(); // Clear the LCD
}


//---------------------------------------------------------------------
//            timerA0On
//---------------------------------------------------------------------
// Function to enable TimerA0 for a specified duration (x milliseconds)
void timerA0On() {
    int temp = x;
    while (temp > 500) {
        enable_TimerA0(); // Enable TimerA0 in up mode
        enterLPM(0); // Enter low power mode
        temp -= 500; // Decrease the remaining time by 500ms
    }
    if (temp != 0) {
        int temp2 = temp;
        temp = temp << 7; // Approximate calculation for the remaining time
        TA0CCR0 = temp + temp2 + temp2 + temp2;
        enable_TimerA0(); // Enable TimerA0 in up mode
        enterLPM(0); // Enter low power mode
        TA0CCR0 = 0xFFFF; // Reset TA0CCR0 to 0xFFFF for 0.5 sec
    }
}

