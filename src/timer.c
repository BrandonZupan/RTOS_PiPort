// timer.c
// Brandon Zupan

#include <stdint.h>

#include "utils.h"
#include "printf.h"
#include "peripherals/timer.h"

uint32_t periods [4];

void (*PeriodicTask1) (void);

void Timer1_Init(uint32_t period, void(*task)(void)) {
    periods[1] = period;
    uint32_t cur_val = get32(TIMER_CLO);
    uint32_t next = cur_val + period;
    put32(TIMER_C1, next);

    PeriodicTask1 = task;
}

void Timer1_Handler(void) {
    uint32_t cur_val = get32(TIMER_CLO);
    cur_val += periods[1];

	put32(TIMER_C1, cur_val);    // update timer trigger time
	put32(TIMER_CS, TIMER_CS_M1);

    PeriodicTask1();
}

uint32_t Timer_CurTime(void) {
    return get32(TIMER_CLO);
}

uint32_t Timer1_Trigger(void) {
    return get32(TIMER_C1);
}