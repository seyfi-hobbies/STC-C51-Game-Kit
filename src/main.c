/*//////////////////////////////////////////////////////////////////////////////////   
Seyfi, start 1/8/2023
Finally working!!  1/13/2013
**************************************************************************************************/   
#include "sys.h"
#include "lcd.h"
#include "game.h"
#include "graph.h"

float time_cnt = 0.0; //global time counter, calibrated to 1ms (approximately)

/**************************************************** *******************************
* Function name: Timer0Init
* Function function: timer 0 initialization
* Input: None
* Output: None
* WARNING: TL0 and TH0 values are calculated for 24Mhz clock frequency, so STC15F2K60S2 must be
*          programmed exactly at this frequency. If you change the clock frequency for some reason
*          recalculate timer values and set it here. (STC-ISP might be used to calculate those values)      
* if the time is off you will see slow moving Flappy Bird or pipe leaving tracks after it     
***************************************************** *******************************/
void Timer0Init()
{
    TMOD |= 0X01;   //Select timer 0 mode, work mode 1, only use TR0 to open and start.
    TL0 = 0x30;		//Initial timer value 
	TH0 = 0xF8;		//Initial timer value
    ET0 = 1;        //Enable timer 0 interrupt enable
    EA = 1;         //Open total interrupt
    TR0 = 1;        //Open timer
}


/**************************************************** *******************************
* Function name: void Timer0() interrupt 1
* Function function: Timer 0 interrupt function
* Input: None
* Output: None
* SAME WARNING HERE. And TL0 and TH0 should be exactly same as above.
***************************************************** *******************************/
void Timer0() __interrupt 1
{
 	TL0 = 0x30;		//Initial timer value
	TH0 = 0xF8;		//Initial timer value
    time_cnt+=1;    
}

/*
* MAIN PROGRAM
*/
void main(void)
{
	Timer0Init(); 
    LCD_Init();
	
    while(1)
    {
		    game_start();
		    game_loop();
		    game_over();
    }
}
