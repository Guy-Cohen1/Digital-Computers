#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include <math.h>

//-------------------------------------------------------------
//            Print SWs value onto LEDs
//-------------------------------------------------------------
void printSWs2LEDs(void){
	unsigned char ch;
	
	ch = readSWs();
	print2LEDs(ch);
}
//--------------------------------------------------------------------
//            Print array to LEDs array with rate of LEDs_SHOW_RATE
//--------------------------------------------------------------------            
void printArr2SWs(char Arr[], int size, unsigned int rate){
	unsigned int i;
	
	for(i=0; i<size; i++){
		print2LEDs(Arr[i]);
		delay(rate);
	}
}

//--------------------------------------------------------------------
//            count up or down on leds - state1
//-------------------------------------------------------------------- 
 
char count_up_down(int *flag, char last_count)
{
  int i=0;
  int j=0;
  LEDsArrPort = last_count;
  if (*flag == 0)
  {
    for (i; i<20; i++)
    {
      incLEDs(1);
      delay(LEDs_SHOW_RATE-5000);    
    }
    last_count = LEDsArrPort;
    print2LEDs(0);
    *flag = 1;
    flag_off(1);
    state = state0;
    return last_count;
  }
   else
    {
    LEDsArrPort = last_count;
    for (j; j<20; j++)
    {
      incLEDs(-1);
      delay(LEDs_SHOW_RATE-5000);
    }
    last_count = LEDsArrPort;
    print2LEDs(0);
    *flag = 0;
    flag_off(1);
    state = state0;
    return last_count;
    } 
}

//--------------------------------------------------------------------
//            move leds - state2
//-------------------------------------------------------------------- 

int move_leds(int last_move)
{
    int i=last_move+1;;
    int counter = 0;
    print2LEDs((char)pow(2,last_move));
    delay(LEDs_SHOW_RATE);
    counter++;
    for (i; i<8; i++)
    {
      print2LEDs((char)pow(2,i));
      delay(LEDs_SHOW_RATE-5000);  
      counter++;
    }
 
    while(counter < 14)
    {
        int k = 0;
        for (k; k<8; k++)
        {
          print2LEDs((char)pow(2,k));
          delay(LEDs_SHOW_RATE-5000);
          counter++;
          if(counter == 14)
          {
              print2LEDs(0);
              state = state0;
              flag_off(2);
              return  k;
          }
          last_move = k;
        }
    }
    state = state0;
    flag_off(2);
    return last_move;
}

//--------------------------------------------------------------------
//            pwm - state3
//-------------------------------------------------------------------- 


int pwm()
{
    flag_off(3);
    while(PBsArrIntPend == 0x80 || PBsArrIntPend == 0x00)  
    {
        print2_7(1);
        delay(17);
        print2_7(0);
        delay(3);
        if(state != state3)
        {
            return 0;
        }
    }
}


//--------------------------------------------------------------------
//            pwm - state3
//-------------------------------------------------------------------- 


void Lab()
{
  //  state = state0;
    //flag_off(2);
}


