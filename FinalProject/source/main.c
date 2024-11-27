#include  "../header/api.h"
#include  "../header/app.h"
#include  <stdio.h>



enum FSMstate state;
enum Stepperstate stateStepp;
enum SYSmode lpm_mode;


void main(void){
  
  state = state0;  // start in idle state on RESET
  stateStepp = initstate;
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();     // Configure GPIO, Init ADC
  lcd_init();
  lcd_clear();



  while(1){
	switch(state){

	case state0: //   StepperUsingJoyStick
	    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	    switch(stateStepp){
	    case rotatestate:
	        while(rotateIFG)
	        {
	            curr_counter++;
	            rotate_clock_direction_calibaration(100);
	            TIMERA0_delay_ms(1000);
	        }
	        break;

        case stoprotatestate:
            break;

        case joystickstate:
            counter = 514;
            StepperUsingJoyStick();
            break;
        case initstate:
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ int until Byte RXed
            break;
	    }
	    break;

	case state1: // Paint
	    JoyStickIntEN |= BIT5;      //PB interrupt
	    while (state == state1){
	        Joystick_sample_for_paint();
	    }
        JoyStickIntEN &= ~BIT5;
	    break;

	case state2: // Calibration
        IE2 |= UCA0RXIE;       // Enable UART interrupt

        switch(stateStepp){
        case initstate:
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ int until Byte RXed
            break;

        case rotatestate: // start rotate
            counter = 0;
            while(rotateIFG)
            {
                counter++; //counter = phi*360
                rotate_clock_direction_calibaration(100);
                TIMERA0_delay_ms(1000);
            }
            break;

        case stoprotatestate: // stop and set phi
            calibrate();
            break;
        }
	    break;

	case state3:  //Script
        IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	    while (state == state3){
	        Script_mode_handler();
	    }
		break;
		
	}
  }
}

  
  
  
  
  
  
