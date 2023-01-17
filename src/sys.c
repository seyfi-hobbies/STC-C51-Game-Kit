
#include "sys.h"

void Timer0Init();

//Didn't use these much, not sure if they are accurate
void delay_ms(int count) ///* X1ms */
{
        int i,j;
        for(i = 0;i<count;i++)
                for(j = 0;j<1000;j++);
}

void delay_us(int count) ///* X1us */
{
        int i,j;
        for(i = 0;i<count;i++)
                for(j = 0;j<1;j++);
}
