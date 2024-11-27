#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
int* flag;
char last_count1 = 0;
int last_move = 0;

void main(void){
  
  *flag = 0;
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
		print2LEDs(0);
                enterLPM(lpm_mode);
		break;
		 
        case state1:
		disable_interrupts();
		delay(LEDs_SHOW_RATE);	// delay of 62.5 [ms]
        last_count1 = count_up_down(flag, last_count1);
		enable_interrupts();
		break;
		 
	  case state2:
		disable_interrupts();
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
                last_move = move_leds(last_move);
		enable_interrupts();
		break;           
                
          case state3:
                
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
                pwm();
		break;
		
          case state4:
                
		delay(LEDs_SHOW_RATE);		// delay of 62.5 [ms]
                Lab();
		break;
	}
  }
}
  
  
  
  
  
  
