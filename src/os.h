// os.h
// Brandon Zupan

#ifndef _OS_H
#define _OS_H

#include <stdint.h>

void OS_InString(char * s, uint32_t max);

void OS_BoyKisser(void);

void OS_Init(void);

uint32_t OS_CurTime(void);

uint32_t OS_Timer1_Trigger(void);

#endif  /*_OS_H*/