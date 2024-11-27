#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  state = state8;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET

  sysConfig();
  lcd_init();
  lcd_clear();

  while(1){
	switch(state){
	    case state8:
        enterLPM(lpm_mode);
		break;

      case state1:
          RGBBLINK();
          break;

	  case state2:
	      STOPER();
	      break;

	  case state3:
          buzzer();
		  break;

      case state4:
          set_X();
          state = state8;
		  break;

      case state5:
          PRINT_POT();
          break;

      case state6:
          LCD_CLR_AND_RESET_X();
          state = state8;
          break;

//      case state7:
//
//          break;
	}
  }
}
