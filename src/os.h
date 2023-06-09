// os.h
// Brandon Zupan

#ifndef _OS_H
#define _OS_H

#include <stdint.h>

#define MAX_STACK_SIZE 1024

typedef struct Sema4 {
    int32_t value;
} Sema4_t;

typedef struct TCB {
    // Reserve space for registers during context switch
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t fp;
    uint64_t sp;
    uint64_t pc;
    struct TCB *next;
    struct TCB *prev;
    uint32_t sleep;
    uint32_t is_valid;
    uint32_t id;
    uint32_t prio;
    Sema4_t *block_pt;
    uint64_t initial_sp;
    // uint64_t stack[MAX_STACK_SIZE];
} TCB_t;

void OS_Sleep(uint32_t time_ms);

uint32_t OS_CurTime(void);

uint32_t OS_Timer1_Trigger(void);

uint64_t OS_NumSwitches(void);

uint32_t OS_AddThread(void (*task)(void), uint64_t stackSize, uint32_t priority);

void OS_Launch(void);

void OS_InString(char * s, uint32_t max);

void OS_BoyKisser(void);

void OS_Counter(uint32_t counter);

uint64_t OS_GetCounter(uint32_t counter);

void OS_DisableScheduler(void);

void OS_EnableScheduler(void);

void OS_PrintTCB(TCB_t * tcb);

void OS_PrintRunPt(void);

void OS_InitSemaphore(Sema4_t *sema_pt, int32_t value);

void OS_bWait(Sema4_t *sema_pt);

void OS_bSignal(Sema4_t *sema_pt);

void OS_Kill(void);

void OS_Init(void);

#endif  /*_OS_H*/