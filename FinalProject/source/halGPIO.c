#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/flash.h"     // private library - FLASH layer
#include "stdio.h"
#include "stdint.h"
#include "string.h"



// Global Variables
char arr_print[16];
char arr_print_temp[16];
int j=0;
char *ptr1, *ptr2, *ptr3;
short FIRSTBYTE_FLAG = 0;
short paint_flag = 1; // 0-state changed -> send state(pb pressed)
int rotateIFG = 1;
unsigned int delay_time = 500;
const unsigned int timer_half_sec = 65535;
unsigned int i = 0;
unsigned int TX_counter;
char last_samples[4];
short Vx_Vy[] = {0, 0}; //Vx_Vy[0]=Vx, Vx_Vy[1]=VY
const short PB_PAINT_str[] = {1000, 1000}; // send if button pressed - state changed
char RxData[80];
char Rec_data[80];
int ExecuteFlag = 0;
int transfer_to_flash = 0;
int SendFlag = 0;
int startRotateLEDs = 0x10;
int* rotateLEDs = &startRotateLEDs;
int counter = 514;
char last_samples[4];
char ACK_string[3] = "FIN";
int curr_counter = 0;
short ACK_to_PC_IPG = 0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	ADCconfig();
	StopAllTimers();
	UART_init();
}

//--------------------------------------------------------------------
//              Send ACK
//--------------------------------------------------------------------
void ACK_to_PC() {
    ACK_to_PC_IPG = 1;  //acknowledgment is being sent to the PC
    TX_counter = 0;
    UCA0TXBUF = ACK_string[TX_counter++];
    IE2 |= UCA0TXIE; //enable interrupts
    __bis_SR_register(LPM0_bits + GIE);
    TIMERA0_delay_ms(10000);
    ACK_to_PC_IPG = 0; //acknowledgment process is complete
}

//--------------------------------------------------------------------
//              Send degree to PC
//--------------------------------------------------------------------
void send_degree_to_PC(){
    TX_counter = 0;
    UCA0TXBUF = last_samples[TX_counter++];
    IE2 |= UCA0TXIE;                        //enable interrupts
    __bis_SR_register(LPM0_bits + GIE);
    TIMERA0_delay_ms(10000);
}

//---------------------------------------------------------------------
//            General Function
//---------------------------------------------------------------------

void convert_integer_to_string(char *str, unsigned int num){
    int strSize = 0;
    long tmp = num, len = 0;
    int j;
    if (tmp == 0){
        str[strSize] = '0';
        return;
    }
    // Find the size of the intPart by repeatedly dividing by 10
    while(tmp){
        len++;
        tmp /= 10;
    }

    // Print out the numbers in reverse
    for(j = len - 1; j >= 0; j--){
        str[j] = (num % 10) + '0';
        num /= 10;
    }
    strSize += len;
    str[strSize] = '\0';
}

//-----------------------------------------------------------------------
uint32_t convert_hex_to_int(char *hex) {
    uint32_t val = 0;
    int o;
    for(o=0; o<2; o++) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}
//-----------------------------------------------------------------------
void move_pointer_to_specific_deg(uint32_t dest_deg, char curr_state)
{
    int k;                // Counter for the number of steps to move
    uint32_t step_counter;         // Total number of steps needed to reach the desired angle
    uint32_t calc_temp;

    // Calculate the total number of steps required to move to the desired angle
    calc_temp = dest_deg * counter;  // Multiply the desired degrees by the step per degree
    step_counter = (calc_temp / 360);        // Convert degrees to steps (assuming 360 degrees per full rotation)

    // Calculate the difference between the current position and the target position
    int diff = step_counter - curr_counter;

    if (0 <= diff) {  // If target position is greater than or equal to current position, move clockwise
        for (k = 0; k < diff; k++) {
            curr_counter++;  // Increment the current position
            rotate_clock_direction(150);  // Rotate stepper motor clockwise
            TIMERA0_delay_ms(10000);  // Delay to allow motor to complete step

            // Send data to PC if the script state is '6' or for stepper degree operation
            if (curr_state == '6') {
                convert_integer_to_string(last_samples, curr_counter);  // Convert current counter value to string
                send_degree_to_PC();  // Send updated degree to PC
            }
        }

        // Additional handling if script state is '7'
        if (curr_state == '7') {
            convert_integer_to_string(last_samples, curr_counter);  // Convert current counter value to string
            convert_to_str(dest_deg);  // Convert target degrees to string
            lcd_home();  // Set LCD cursor to home position
            lcd_clear();  // Clear LCD display
            lcd_puts(arr_print);  // Display the target degrees on LCD
        }

        // Add finish flag after moving to the target degree
        sprintf(last_samples, "%s", "FFFF");  // Set finish flag
    }
    else {  // If target position is less than current position, move counter-clockwise
        for (k = diff; k < 0; k++) {
            curr_counter--;  // Decrement the current position
            rotate_anti_clock_direction(150);  // Rotate stepper motor counter-clockwise
            TIMERA0_delay_ms(10000);  // Delay to allow motor to complete step

            // Send data to PC if the script state is '6' or for stepper degree operation
            if (curr_state == '6') {
                convert_integer_to_string(last_samples, curr_counter);  // Convert current counter value to string
                send_degree_to_PC();  // Send updated degree to PC
            }
        }

        // Additional handling if script state is '7'
        if (curr_state == '7') {
            convert_integer_to_string(last_samples, curr_counter);  // Convert current counter value to string
            convert_to_str(dest_deg);  // Convert target degrees to string
            lcd_puts(arr_print);  // Display the target degrees on LCD
        }

        // Add finish flag after moving to the target degree
        sprintf(last_samples, "%s", "FFFF");  // Set finish flag
    }
}

//----------------------Count Timer Calls---------------------------------
void timer_call_counter(){
    unsigned int num_of_halfSec;  // Number of full half-second intervals needed
    unsigned int res;             // Remaining time after full half-second intervals
    // Calculate the number of half-second intervals
    num_of_halfSec = (int) delay_time / half_sec;
    // Calculate the remaining time after accounting for full half-second intervals
    res = delay_time % half_sec;
    // Scale the remaining time to the timer's clock resolution
    res = res * clk_tmp;
    // Generate the full half-second intervals
    for (i = 0; i < num_of_halfSec; i++){
        TIMER_A0_config(timer_half_sec);  // Configure Timer_A0 for a half-second delay
        __bis_SR_register(LPM0_bits + GIE);  // Enter Low Power Mode 0 with interrupts enabled
        // The CPU will wake up when Timer_A0 triggers an interrupt
    }

    // Handle any remaining time that does not fit into a full half-second interval
    if (res > 1000){
        TIMER_A0_config(res);  // Configure Timer_A0 for the remaining time
        __bis_SR_register(LPM0_bits + GIE);  // Enter Low Power Mode 0 with interrupts enabled
        // The CPU will wake up when Timer_A0 triggers an interrupt
    }
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
//            Start Timer With counter
//---------------------------------------------------------------------
void TIMERA0_delay_ms(unsigned int counter){
    TIMER_A0_config(counter);
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
//---------------------------------------------------------------------
//            Start Timer1 With counter
//---------------------------------------------------------------------
void START_TIMERA1(unsigned int counter){
    TIMER_A1_config(counter);
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
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
        enable_interrupts();

}

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
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm("NOP");
  LCD_EN(0);
}

//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}


//---------------------------------------------------------------------
//            convert_to_str
//---------------------------------------------------------------------

void convert_to_str(int num){
  int temp = num;
  int i = 0;
  while(temp!=0){
    temp = temp/10;
    i++;
  }
  while(i>0){
      arr_print[i-1] = (num%10+'0');
      num = num/10;
      i--;
    }
}


// ------------------------------------------------------------------
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
//                        TIMER A0 ISR
//*********************************************************************
#pragma vector = TIMER0_A0_VECTOR // For delay
__interrupt void TimerA_ISR (void)
{
    StopAllTimers();
    LPM0_EXIT;
}

//*********************************************************************
//                        TIMER A ISR
//*********************************************************************
#pragma vector = TIMER1_A0_VECTOR // For delay
__interrupt void Timer1_A0_ISR (void)
{
    if(!TAIFG) { StopAllTimers();
    LPM0_EXIT;
    }
}

//*********************************************************************
//                         ADC10 ISR
//*********************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
   LPM0_EXIT;  // Exit Low Power Mode 0
}

//-------------------ATAN2- Fixed point - returns degrees---------------------------
int16_t tangens(int16_t y_fp, int16_t x_fp)
{
    // Coefficients for fixed-point calculations
    int32_t coeff_1 = 45;        // Coefficient for fixed-point atan2 approximation
    int32_t coeff_1b = -56;      // Coefficient for fixed-point atan2 approximation
    int32_t coeff_1c = 11;       // Coefficient for fixed-point atan2 approximation
    int16_t coeff_2 = 135;       // Additional coefficient used for final angle calculation

    int16_t angle = 0;           // Variable to hold the computed angle

    int32_t r;                   // Temporary variable to hold intermediate result
    int32_t r3;                  // Variable to hold r^3

    int16_t y_abs_fp = y_fp;     // Absolute value of y_fp
    if (y_abs_fp < 0)
        y_abs_fp = -y_abs_fp;    // Make y_abs_fp positive if it's negative

    // Determine angle based on the quadrant of (x_fp, y_fp)
    if (y_fp == 0)
    {
        // If y_fp is zero, angle is either 0 or 180 degrees based on x_fp
        if (x_fp >= 0)
        {
            angle = 0;   // Positive x-axis
        }
        else
        {
            angle = 180; // Negative x-axis
        }
    }
    else if (x_fp >= 0)
    {
        // Case where x_fp is non-negative (Quadrants I or IV)
        r = (((int32_t)(x_fp - y_abs_fp)) << MULTIPLY_FP_RESOLUTION_BITS) /
            ((int32_t)(x_fp + y_abs_fp)); // Compute r for angle calculation

        r3 = r * r;                       // Calculate r^2
        r3 = r3 >> MULTIPLY_FP_RESOLUTION_BITS; // Adjust for fixed-point resolution
        r3 *= r;                         // Calculate r^3
        r3 = r3 >> MULTIPLY_FP_RESOLUTION_BITS; // Adjust for fixed-point resolution
        r3 *= coeff_1c;                  // Multiply by coefficient
        angle = (int16_t) (coeff_1 + ((coeff_1b * r + r3) >> MULTIPLY_FP_RESOLUTION_BITS)); // Calculate final angle
    }
    else
    {
        // Case where x_fp is negative (Quadrants II or III)
        r = (((int32_t)(x_fp + y_abs_fp)) << MULTIPLY_FP_RESOLUTION_BITS) /
            ((int32_t)(y_abs_fp - x_fp)); // Compute r for angle calculation
        r3 = r * r;                       // Calculate r^2
        r3 = r3 >> MULTIPLY_FP_RESOLUTION_BITS; // Adjust for fixed-point resolution
        r3 *= r;                         // Calculate r^3
        r3 = r3 >> MULTIPLY_FP_RESOLUTION_BITS; // Adjust for fixed-point resolution
        r3 *= coeff_1c;                  // Multiply by coefficient
        angle = coeff_2 + ((int16_t) (((coeff_1b * r + r3) >> MULTIPLY_FP_RESOLUTION_BITS))); // Calculate final angle
    }

    // Adjust angle based on the sign of y_fp
    if (y_fp < 0)
        return (360 - angle);  // Invert angle if y_fp is negative (Quadrants III or IV)
    else
        return angle;         // Return calculated angle for other cases
}


//***********************************TX ISR******************************************
// Interrupt Service Routine (ISR) for USCI0 TX (Transmit) vector.
// This ISR is triggered when the UART transmit buffer is ready to accept new data.
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Handle data transmission for script completion
    if (state == state3 && ACK_to_PC_IPG == 1) {
        UCA0TXBUF = ACK_string[TX_counter++]; // send ACK to pc

        // Check if all characters have been transmitted
        if (TX_counter == sizeof(ACK_string) - 1) {
            TX_counter = 0; // Reset the index for next transmission
            IE2 &= ~UCA0TXIE;
            stateStepp = initstate; // Reset the stepping state
            LPM0_EXIT; // Exit Low Power Mode 0
        }
    }

    // Handle data transmission for script steps
    if (state == state3 && ACK_to_PC_IPG == 0) {
        UCA0TXBUF = last_samples[TX_counter++]; // Transmit the next character from last_samples

        // Check if all characters have been transmitted
        if (TX_counter == sizeof(last_samples) - 1) {
            TX_counter = 0; // Reset the index for next transmission
            IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt to stop further transmissions
            stateStepp = initstate; // Reset the stepping state
            LPM0_EXIT; // Exit Low Power Mode 0
        }
    }

    // Handle data transmission for counter in calibration mode
    else if (state == state2 && stateStepp == stoprotatestate) {
        UCA0TXBUF = last_samples[TX_counter++]; // Transmit the counter to pc

        // Check if all characters have been transmitted
        if (TX_counter == sizeof(last_samples) - 1) {
            TX_counter = 0; // Reset the index for next transmission
            IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt to stop further transmissions
            stateStepp = initstate; // Reset the stepping state
            LPM0_EXIT; // Exit Low Power Mode 0
        }
    }

    // Handle data transmission when painting with joystick (paint_flag is set)
    else if (paint_flag && state == state1) {
        if (FIRSTBYTE_FLAG)
            UCA0TXBUF = (PB_PAINT_str[i++] >> 8) & 0xFF; // Transmit the high byte
        else
            UCA0TXBUF = PB_PAINT_str[i] & 0xFF; // Transmit the low byte
        FIRSTBYTE_FLAG ^= 1; // Toggle the flag to switch between high and low bytes
        // Check if both bytes have been transmitted
        if (i == 2) {
            i = 0; // Reset the index for next transmission
            IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt to stop further transmissions
            TIMERA0_delay_ms(10000); // Wait for a delay to allow synchronization
            paint_flag = 0; // Reset the paint flag
            LPM0_EXIT; // Exit Low Power Mode 0
        }
    }

    // Handle data transmission when painting with joystick (paint_flag is not set)
    else if (!paint_flag && state == state1) {//no need to change between tools
        // Alternate between sending high and low bytes of Vx_Vy[i]
        if (FIRSTBYTE_FLAG)
            UCA0TXBUF = (Vx_Vy[i++] >> 8) & 0xFF; // Transmit the high byte
        else
            UCA0TXBUF = Vx_Vy[i] & 0xFF; // Transmit the low byte
        FIRSTBYTE_FLAG ^= 1; // Toggle the flag to switch between high and low bytes
        // Check if both bytes have been transmitted
        if (i == 2) {
            i = 0; // Reset the index for next transmission
            IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt to stop further transmissions
            TIMERA0_delay_ms(10000); // Wait for a delay to allow synchronization
            LPM0_EXIT; // Exit Low Power Mode 0
        }
    }
}

//***********************************RX ISR******************************************
// Interrupt Service Routine (ISR) for USCI0 RX (Receive) vector.
// This ISR is triggered when data is received via the UART interface.
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    RxData[j] = UCA0RXBUF;
    j++;
    if (RxData[j-1] == 'Z') {  // the end of the string
        j = 0;
        SendFlag = 0; // flag indicating if a file content is being sent
        strcpy(Rec_data, RxData);
    }

    if (!SendFlag && RxData[j-1] == '\x0a') { //indicates EOF
        for (i = 0; i < j; i++) {
            file.file_name[i] = RxData[i]; // Copy the received file name from RxData to file.file_name
        }
        SendFlag = 1; // file received
        j = 0;
    }
    if (RxData[j-1] == 'W') {
        transfer_to_flash = 1; // Set flag to indicate flash operation
        ptr1 = (char*) 0x1000; // Set pointer to the address for the 1st file
        file.file_ptr[0] = ptr1; // Assign pointer to the file_ptr array
        file.files_amount = 1; // Update the number of files
        j = 0; // Reset the index for RxData
    }
    if (RxData[j-1] == 'X') { // Pointer for the 2nd selected file
        transfer_to_flash = 1; // Set flag to indicate flash burn operation
        ptr2 = (char*) 0x1040; // Set pointer to the address for the 2nd file
        file.file_ptr[1] = ptr2; // Assign pointer to the file_ptr array
        file.files_amount = 2; // Update the number of files
        j = 0; // Reset the index for RxData
    }
    if (RxData[j-1] == 'Y') { // Pointer for the 3rd selected file
        transfer_to_flash = 1; // Set flag to indicate flash burn operation
        ptr3 = (char*) 0x1080; // Set pointer to the address for the 3rd file
        file.file_ptr[2] = ptr3; // Assign pointer to the file_ptr array
        file.files_amount = 3; // Update the number of files
        j = 0; // Reset the index for RxData
    }

    // Check if the received character is 'T', 'U', or 'V' to set the execution index
    if (RxData[j-1] == 'T') { // Index for the executed list (1 file)
        ExecuteFlag = 1; // Set flag to indicate that execution should begin
        j = 0; // Reset the index for RxData
        file.files_amount = 1; // Update the number of files to 1
    }
    if (RxData[j-1] == 'U') { // Index for the executed list (2 files)
        ExecuteFlag = 1; // Set flag to indicate that execution should begin
        j = 0; // Reset the index for RxData
        file.files_amount = 2; // Update the number of files to 2
    }
    if (RxData[j-1] == 'V') { // Index for the executed list (3 files)
        ExecuteFlag = 1; // Set flag to indicate that execution should begin
        j = 0; // Reset the index for RxData
        file.files_amount = 3; // Update the number of files to 3
    }

    // Check for state commands to update the state machine
    if (RxData[0] == 'm') { // Command to set state to 'manual'
        state = state0; // Set the system state to manual
        stateStepp = initstate; // Initialize the stepping state
        rotateIFG = 0; // Clear the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }
    else if (RxData[0] == 'P') { // Command to set state to 'painter'
        state = state1; // Set the system state to painter mode
        stateStepp = initstate; // Initialize the stepping state
        rotateIFG = 0; // Clear the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }
    else if (RxData[0] == 'C') { // Command to set state to 'calibration'
        state = state2; // Set the system state to calibration mode
        stateStepp = initstate; // Initialize the stepping state
        rotateIFG = 0; // Clear the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }
    else if (RxData[0] == 's') { // Command to set state to 'script mode'
        state = state3; // Set the system state to script mode
        stateStepp = initstate; // Initialize the stepping state
        rotateIFG = 0; // Clear the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }

    // Check for commands to control motor rotation
    if (RxData[0] == 'A') { // Command to auto-rotate
        stateStepp = rotatestate; // Set the stepping state for auto-rotation
        rotateIFG = 1; // Set the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }
    else if (RxData[0] == 'M') { // Command to stop rotation
        stateStepp = stoprotatestate; // Set the stepping state to stop rotation
        rotateIFG = 0; // Clear the rotation interrupt flag
        j = 0; // Reset the index for RxData
    }
    else if (RxData[0] == 'J') { // Command to rotate with joystick
        stateStepp = joystickstate; // Set the stepping state to joystick control
        j = 0; // Reset the index for RxData
    }

    // Exit Low Power Mode 0 and enable interrupts
    LPM0_EXIT;
}

//*********************************************************************
//            Port1 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void Joystick_handler(void){
      IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
      delay(debounceVal);

      if(JoyStickIntPend & BIT5){ //int at P1.5
          paint_flag = 1; // send state!
          JoyStickIntPend &= ~BIT5;
      }
      IE2 |= UCA0TXIE;                       // enable USCI_A0 TX interrupt
}
