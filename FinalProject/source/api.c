
#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/flash.h"     // private library - FLASH layer
#include "stdio.h"
#include "math.h"


int wait_to_execute = 1;
int counter3 = 0;
int counter4 = 0;
int counter5 = 0;
const unsigned int timer_half_sec2 = 65535;
int16_t Vrx = 0;
int16_t Vry = 0;


//---------------------------------------------------------------------
//            countUP
//---------------------------------------------------------------------
void countUP(){
    //lcd_clear();  // Clear the LCD screen
    lcd_home();   // Move the cursor to the home position
    cursor_off;   // Turn off the cursor
    //while(state == state3 && counter3 <= num){  // Loop while the state is state2
    // Convert integer countUp to string and display on LCD
    convert_to_str(counter3);
    if(counter3 != 0)
         lcd_clear();
    lcd_home();
    lcd_puts(arr_print);
    counter3++;   // Increment countUp
    lcd_home();   // Move the cursor to the home position
    timer_call_counter();
    //}
}

//---------------------------------------------------------------------
//            countDOWN
//---------------------------------------------------------------------
void countDOWN(){
    //lcd_clear();  // Clear the LCD screen
    lcd_home();   // Move the cursor to the home position
    cursor_off;   // Turn off the cursor
    // Convert integer countUp to string and display on LCD
    convert_to_str(counter4);
    if(counter4 != 0)
        lcd_clear();
    lcd_home();
    lcd_puts(arr_print);
    counter4--;   // decrement countUp
    lcd_home();   // Move the cursor to the home position
    timer_call_counter();
}



//---------------------------------------------------------------------
//            rrc_LCD
//---------------------------------------------------------------------

void rra_LCD(int num)
{
  // Check if the display has fewer than 16 characters
  if(counter5 < 16)
  {
      // If there are already some characters on the display, shift them left
      if(counter5 > 0)
      {
          lcd_cursor_left();   // Move the cursor left
          lcd_putchar(' ');    // Replace the current character with a space
      }
      lcd_putchar(num + 0x30); // Display the new number (convert to ASCII)
      counter5++;              // Increment the character count
      timer_call_counter();   // Call a function to handle timing
  }
  else if(counter5 == 16)
  {
      // If the display is full, shift all characters left, and move to a new line
      lcd_cursor_left();       // Move the cursor left
      lcd_putchar(' ');        // Replace the last character with a space
      lcd_new_line;           // Move the cursor to the next line
      lcd_putchar(num + 0x30); // Display the new number
      counter5++;             // Increment the character count
      timer_call_counter();  // Call a function to handle timing
  }
  else
  {
      // If counter5 exceeds 16, shift all characters left and replace with new number
      lcd_cursor_left();       // Move the cursor left
      lcd_putchar(' ');        // Replace the previous character with a space
      lcd_putchar(num + 0x30); // Display the new number
      counter5++;              // Increment the character count
      timer_call_counter();   // Call a function to handle timing
  }
}

//---------------------------------------------------------------------
//            reset Count and Clr LCD
//---------------------------------------------------------------------

void rst_count_lcd_clr(){
    int l;
    // Clear the arr_print array
    for(l = 0; l < 16; l++){
        arr_print[l] = 0; // Set each element to 0
    }
    // Reset all counters to 0
    counter3 = 0;
    counter4 = 0;
    counter5 = 0;
    lcd_clear(); // Clear the LCD display
}

//-------------------------------------------------------------
//                Joystick_sample_for_paint
//-------------------------------------------------------------
void Joystick_sample_for_paint() {
    JoyStickIntEN &= ~BIT5; // Disable interrupt of PB
    i = 0;
    if (!paint_flag) {     // Check if paint_flag is not set (PB)
        ADC10CTL0 &= ~ENC; // Disable ADC10 conversion by clearing ENC (Enable Conversion) bit
        while (ADC10CTL1 & ADC10BUSY); // Wait for ADC10 to finish any ongoing conversion by checking if ADC10 is busy
        ADC10SA = &Vx_Vy;
        ADC10CTL0 |= ENC + ADC10SC; // Start ADC sampling and conversion
        __bis_SR_register(LPM0_bits + GIE);
        UCA0TXBUF = Vx_Vy[i] & 0xFF;// Transmit the least significant byte of the ADC result (Vx_Vy[i]) via UART
        FIRSTBYTE_FLAG = 1;
        IE2 |= UCA0TXIE;// Enable UART transmit interrupt
        __bis_SR_register(LPM0_bits + GIE);
    }
    else if (paint_flag) {
        UCA0TXBUF = PB_PAINT_str[i] & 0xFF; // Transmit the least significant byte of the state change data (PB_PAINT_str[i]) via UART
        FIRSTBYTE_FLAG = 1;
        IE2 |= UCA0TXIE; // Enable UART transmit interrupt
        __bis_SR_register(LPM0_bits + GIE);
        TIMERA0_delay_ms(5000);
        JoyStickIntPend &= ~BIT5;
    }
    JoyStickIntEN |= BIT5; // Enable interrupt only at the end of the cycle
}

//-------------------------------------------------------------
//                JoyStickADC_Steppermotor
//-------------------------------------------------------------
void JoyStickADC_Steppermotor(){
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & ADC10BUSY);               // Wait if ADC10 core is active
        ADC10SA = &Vx_Vy;                        // Data buffer start
        ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
        __bis_SR_register(LPM0_bits + GIE);        // LPM0, ADC10_ISR will force exit

}

//-------------------------------------------------------------
//                Script_mode_handler
//-------------------------------------------------------------

void Script_mode_handler() {
    if (transfer_to_flash) { // Check if a flash transfer operation is requested
        transfer_to_flash = 0;
        FCTL2 = FWKEY + FSSEL0 + FN1;
        file.file_size[file.files_amount - 1] = strlen(Rec_data) - 1; // Set the file size of the most recent file to the length of the received data minus 1
        transfer_file_to_flash(); // Write the segment to Flash memory
        ACK_to_PC();  // Send acknowledgment to the PC indicating that the transfer is complete
        // Enable receive interrupt for the UART (UCA0RXIE)
        IE2 |= UCA0RXIE;
    }
    if (ExecuteFlag) { // Check if the script execution flag is set
        ExecuteFlag = 0;
        // Set the script execution flag to indicate that the script should be executed
        wait_to_execute = 1;
        // Set default delay time for script execution
        delay_time = 500;
        // Execute the script
        start_execution();
        // Send acknowledgment to the PC indicating that the script execution is complete
        ACK_to_PC();
    }
    // Enter Low Power Mode 0 with global interrupts enabled
    __bis_SR_register(LPM0_bits + GIE);
}

//---------------Execute Script--------------------------------
void start_execution(void)
{
    int k = 0;  // Loop counter for operations
    char *ptr_start_of_flash;                         // Pointer to navigate through the flash memory segment
    char ofcode_field[10], first_argument[20], second_argument[20];  // Buffers to hold instruction opcode and operands
    int argument_int, X, start, stop, y;

    if (wait_to_execute)  // Check if the script is ready to execute
        ptr_start_of_flash = file.file_ptr[file.files_amount - 1];  // Point to the script data in flash memory
    wait_to_execute = 0;  // Clear the flag indicating readiness for execution

    // Process up to 64 bytes of script data
    for (y = 0; y < 64;)
    {
        // Read the opcode (operation code) from the flash memory
        ofcode_field[0] = *ptr_start_of_flash++;
        ofcode_field[1] = *ptr_start_of_flash++;
        y = y + 2;  // Increment the counter by 2 for opcode bytes

        // Execute based on the opcode value
        switch (ofcode_field[1])
        {
        case '1':  // Opcode '1' indicates a count-up operation
            lcd_clear();  // Clear the LCD display
            // Read the operand for the count-up operation
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            argument_int = convert_hex_to_int(first_argument);  // Convert operand from hex to integer
            counter3 = 0;  // Reset counter
            // Copy temp_amount_of_stepsorary array values to the main display array
            for (k; k < 16; k++)
            {
                arr_print[k] = arr_print_temp[k];
            }
            k = 0;
            lcd_clear();  // Clear the LCD display
            // Perform count-up operation
            while (argument_int >= 0)
            {
                if (counter3 == 0)
                {
                    lcd_clear();
                    lcd_home();
                    lcd_putchar('0');
                    lcd_home();
                    timer_call_counter();
                }
                countUP();  // Perform count-up operation
                argument_int--;  // Decrement operand
            }
            break;

        case '2':  // Opcode '2' indicates a count-down operation
            lcd_clear();
            // Read the operand for the count-down operation
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            argument_int = convert_hex_to_int(first_argument);  // Convert operand from hex to integer
            counter4 = argument_int;  // Set counter4 to operand value
            // Copy temp_amount_of_stepsorary array values to the main display array
            for (k; k < 16; k++)
            {
                arr_print[k] = arr_print_temp[k];
            }
            k = 0;
            lcd_clear();  // Clear the LCD display
            // Perform count-down operation
            while (argument_int >= 0)
            {
                countDOWN();  // Perform count-down operation
                // Optional: rotate LEDs (commented out)
                argument_int--;  // Decrement operand
            }
            lcd_clear();  // Clear the LCD display
            lcd_home();   // Set LCD cursor to home position
            if (counter4 == argument_int)
                lcd_putchar('0');  // Display '0' on LCD if condition is met
            lcd_home();   // Set LCD cursor to home position again
            timer_call_counter();  // Call timer function
            break;

        case '3':  // Opcode '3' indicates a rotate-right operation
            lcd_clear();  // Clear the LCD display
            // Read the operand for the rotate-right operation
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            argument_int = convert_hex_to_int(first_argument);  // Convert operand from hex to integer
            lcd_clear();  // Clear the LCD display
            // Perform rotate-right operation
            while (counter5 < 32)
            {
                // Optional: rotate LEDs (commented out)
                rra_LCD(argument_int);  // Perform rotate-right on LCD
            }
            break;

        case '4':  // Opcode '4' indicates setting a delay time
            lcd_clear();  // Clear the LCD display
            // Read the operand for delay time
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            delay_time = convert_hex_to_int(first_argument);  // Convert operand from hex to integer
            delay_time = delay_time * 10;  // Convert delay time to milliseconds (multiplying by 10)
            break;

        case '5':  // Opcode '5' indicates resetting the LCD count
            rst_count_lcd_clr();  // Reset LCD count display
            break;

        case '6':  // Opcode '6' indicates pointing the stepper motor to a specific degree
            lcd_clear();  // Clear the LCD display
            // Read the operand for motor position
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            X = convert_hex_to_int(first_argument);  // Convert operand from hex to integer
            lcd_clear();  // Clear the LCD display
            move_pointer_to_specific_deg(X, ofcode_field[1]);  // Move motor to specified position
            break;

        case '7':  // Opcode '7' indicates scanning an area between two angles
            lcd_clear();  // Clear the LCD display
            // Read the start and stop angles for scanning
            first_argument[0] = *ptr_start_of_flash++;
            first_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for operand bytes
            second_argument[0] = *ptr_start_of_flash++;
            second_argument[1] = *ptr_start_of_flash++;
            y = y + 2;  // Increment the counter by 2 for second operand bytes
            X = convert_hex_to_int(first_argument);  // Convert start angle from hex to integer
            start = X;  // Set the start angle
            lcd_clear();  // Clear the LCD display
            move_pointer_to_specific_deg(X, ofcode_field[1]);  // Move motor to start angle
            X = convert_hex_to_int(second_argument);  // Convert stop angle from hex to integer
            stop = X;  // Set the stop angle
            move_pointer_to_specific_deg(X, ofcode_field[1]);  // Move motor to stop angle
            break;

        case '8':  // Opcode '8' indicates a sleep operation
            __bis_SR_register(LPM0_bits + GIE);
            break;
        }
    }
}


//*************************************************************
//*************************************************************
//-------------------------------------------------------------
//                Stepper clockwise
//-------------------------------------------------------------
void rotate_clock_direction(long freq_op_pointer){
    StepmotorPortOUT = 0x01;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x08;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x04;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x02;
}

//-------------------------------------------------------------
//                Stepper clockwise
//-------------------------------------------------------------
void rotate_clock_direction_calibaration(long freq_op_pointer){
    StepmotorPortOUT = 0x01;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x08;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x04;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x02;
}

//-------------------------------------------------------------
//                Stepper counter-clockwise
//-------------------------------------------------------------
void rotate_anti_clock_direction(long freq_op_pointer){
    StepmotorPortOUT = 0x08;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x01;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x02;
    TIMERA0_delay_ms(1000);
    StepmotorPortOUT = 0x04;
}


//-------------------------------------------------------------
//                Stepper Motor Calibration
//-------------------------------------------------------------
void calibrate() {
    // Convert the integer 'last_samples' to a string representation
    convert_integer_to_string(last_samples, counter);
    // Initialize the transmission index
    TX_counter = 0;
    // Load the first character of the 'last_samples' string into the UART transmit buffer
    UCA0TXBUF = last_samples[TX_counter++];
    // Enable the USCI_A0 TX interrupt to handle subsequent bytes of data
    IE2 |= UCA0TXIE;
    // Enter Low Power Mode 0 with general interrupts enabled
    __bis_SR_register(LPM0_bits + GIE);
    // Reset the current counter value
    curr_counter = 0;
}

//-------------------------------------------------------------
//                Stepper Using JoyStick
//-------------------------------------------------------------
void StepperUsingJoyStick(){
    uint32_t counter_phi;
    uint32_t phi;          // Angle calculated using joystick values
    uint32_t temp_amount_of_steps;

    while (counter != 0 && state == state0 && stateStepp == joystickstate){
        JoyStickADC_Steppermotor();  // Read joystick ADC values

        if (!(Vx_Vy[1] > 400 && Vx_Vy[1] < 600 && Vx_Vy[0] > 400 && Vx_Vy[0] < 600)){ // Check if joystick values are out of the neutral range
            // Calculate joystick offsets from neutral position
            Vrx = Vx_Vy[1] - 512;
            Vry = Vx_Vy[0] - 512;

            // Calculate the angle (phi) in degrees using joystick offsets
            phi = tangens(Vry, Vrx);
            temp_amount_of_steps = phi * counter;  // Compute the temp_amount_of_steps value based on the angle

            // Adjust the counter value based on the angle
            if (270 < phi) {
                // For angles greater than 270 degrees
                counter_phi = ((counter * 7) / 4) - (temp_amount_of_steps / 360);  // Adjusted counter calculation
            }
            else {
                // For angles less than or equal to 270 degrees
                counter_phi = ((counter * 3) / 4) - (temp_amount_of_steps / 360);  // Adjusted counter calculation
            }

            // Determine the direction to rotate based on current and target counter values
            if ((int)(curr_counter - counter_phi) < 0) {
                rotate_clock_direction(600);  // Rotate motor clockwise
                curr_counter++;               // Increment current counter
            }
            else {
                rotate_anti_clock_direction(600);  // Rotate motor counterclockwise
                curr_counter--;                    // Decrement current counter
            }
        }
    }
}





