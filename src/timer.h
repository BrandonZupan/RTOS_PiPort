// timer.h
// Brandon Zupan

#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

#define TIME_1MS    1000
#define TIME_1S     1000000


void Timer1_Init(uint32_t period, void(*task)(void));

void Timer1_Handler(void);

uint32_t Timer_CurTime(void);

uint32_t Timer1_Trigger(void);


#endif /*_TIMER_H*/