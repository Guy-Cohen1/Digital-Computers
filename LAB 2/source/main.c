#include  "../header/api.h"
#include  "../header/app.h"

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  state = state0;
  lpm_mode = mode0;
  sysConfig();
  lcd_init();
  lcd_clear();
  
  while(1){
	switch(state){
	  case state0:
                __bis_SR_register(LPM0_bits + GIE);
		break;
	
          case state1:
                findHz();  
                lcd_clear();
		break;
                
        case state2:
                UpPauseCount();
                lcd_clear();
                if(state == state2) state = state0;
		break;
		 
	  case state3:
                buzzer_gen();
		break;
                
          case state4:
                
		break;
		
	}
  }
}
