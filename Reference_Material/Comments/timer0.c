/*****************************************************************************
 
*****************************************************************************/
#include "main.h"
volatile unsigned int CountMilliseconds = 0;
volatile unsigned char Timer0Overflow;
 
enum {
  STOP             = 0,
  CK               = 1,
  CK8              = 2,
  CK64             = 3,
  CK256            = 4,
  CK1024           = 5,
  T0_FALLING_EDGE  = 6,
  T0_RISING_EDGE   = 7
};

/*
 * 定时器T0的溢出中断服务程序
 */
SIGNAL(SIG_OVERFLOW0)
{
 static unsigned char cnt;
 Timer0Overflow++;
 if(!cnt--)
  {
   cnt = 3;
   CountMilliseconds += 1;
   if(I2C_Timeout) I2C_Timeout--;
   if(PPM_Timeout) PPM_Timeout--; else anz_ppm_werte = 0;
   if(SIO_Timeout) SIO_Timeout--;
  }
}


void Timer0_Init(void)
{
 TCCR0  = TIMER_TEILER; //t0为clk的8分频
// TCNT0 = -TIMER_RELOAD_VALUE;  // reload
 TIM0_START;
 TIMER2_INT_ENABLE;
}


unsigned int SetDelay(unsigned int t)
{
  return(CountMilliseconds + t - 1);     //CountMilliseconds是全局变量                             
}

char CheckDelay (unsigned int t)
{
  return(((t - CountMilliseconds) & 0x8000) >> 8); //时间到了就返回一个很大的数，时间没到就返回0
}

void Delay_ms(unsigned int w)
{
 unsigned int akt;
 akt = SetDelay(w);
 while (!CheckDelay(akt));
}
