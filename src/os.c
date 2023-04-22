// os.c
// Brandon Zupan

#include <stdint.h>
#include <stddef.h>

#include "printf.h"
#include "display.h"
#include "furry.h"
#include "os.h"
#include "mini_uart.h"
#include "uart.h"
#include "timer.h"
#include "irq.h"

#define ROUND_ROBIN

#define MAX_THREADS 6

extern void __SwitchTask(TCB_t * prev, TCB_t * next);
extern void __StartOS(TCB_t * run_pt);
extern void ret_from_fork(void);

TCB_t ThreadsLL[MAX_THREADS];
TCB_t * RunPt;

uint64_t num_switches = 0;

void putc(void * p, char c) {
    // output both to UART and display
    Display_putc(c);
    uart_send(c);
}

int findFirstFreeThread(void){
  for(int i = 0; i < MAX_THREADS; i++){
    if(!ThreadsLL[i].is_valid) {
        return i;
    }
  }
  return MAX_THREADS;
}

TCB_t * FindNextTask(void) {
    TCB_t * current = RunPt;

    #ifdef ROUND_ROBIN
    // find next task that is not sleeping and not blocked
    do {
        current = current->next;
    } while ((current->sleep != 0) || (current->block_pt != NULL));

    return current;
    #endif

    return NULL;
}

void SwitchTask(TCB_t * next) {
    if (next == RunPt) {
        enable_irq();
        return;
    }
    TCB_t * prev = RunPt;
    RunPt = next;
    printf("Calling __SwitchTask\r\n");
    __SwitchTask(prev, next);
}

// Called every timer tick, so prob 1ms
// Gets next task and runs it
void Scheduler(void) {
    disable_irq();
    num_switches++;
    TCB_t * next_task = FindNextTask();
    printf("Next task ID: %d\r\n", next_task->id);
    SwitchTask(next_task);
    // enable_irq();    // done in __SwitchTask()
}

// Called around every MS
void SleepDecriment(void) {
    // Iterate through TCB array and decriment sleep times
    for (int i = 0; i < MAX_THREADS; i++) {
        if (ThreadsLL[i].is_valid) {
            if (ThreadsLL[i].sleep > 0) {
                ThreadsLL[i].sleep--;
            }
        }
    }
}

// Called every timer tick
void TimerTick(void) {
    printf("TimerTick...\r\n");
    Scheduler();
    SleepDecriment();
}

void OS_Sleep(uint32_t time_ms) {
    RunPt->sleep = time_ms;
    Scheduler();
}

uint32_t OS_CurTime(void) {
    return Timer_CurTime();
}

uint32_t OS_Timer1_Trigger(void) {
    return Timer1_Trigger();
}

uint64_t OS_NumSwitches(void) {
    return num_switches;
}


uint32_t OS_AddThread(void (*task)(void), uint64_t stackSize, uint32_t priority) {
    static uint32_t next_id = 0;

    disable_irq();
    int first_free_thread = findFirstFreeThread();
    if (first_free_thread >= MAX_THREADS) {
        return 0;
    }

    ThreadsLL[first_free_thread].is_valid = 1;

    // ThreadsLL[first_free_thread].sp = (uint64_t) ThreadsLL->stack;
    // ThreadsLL[first_free_thread].pc = (uint64_t) task;
    ThreadsLL[first_free_thread].x19 = (uint64_t) task;
    ThreadsLL[first_free_thread].x20 = 0x2020202020202020;   // arg
    ThreadsLL[first_free_thread].pc = (uint64_t) ret_from_fork;
    ThreadsLL[first_free_thread].sp = (uint64_t) &ThreadsLL->stack[0];


    ThreadsLL[first_free_thread].sleep = 0;
    ThreadsLL[first_free_thread].id = next_id++;
    ThreadsLL[first_free_thread].prio = priority;
    ThreadsLL[first_free_thread].block_pt = NULL;

    if (RunPt == NULL) {
        // this is the first thread
        RunPt = &ThreadsLL[first_free_thread];
        RunPt->next = RunPt;
        RunPt->prev = RunPt;
    } else {
        #ifdef ROUND_ROBIN
        TCB_t *temp_next = RunPt->next;

        RunPt->next = &ThreadsLL[first_free_thread];    // run it next
        ThreadsLL[first_free_thread].next = temp_next;
        ThreadsLL[first_free_thread].prev = RunPt;
        temp_next->prev = &ThreadsLL[first_free_thread];

        #endif
    }

    enable_irq();

    printf("RunPt: 0x%08X\r\n", (uint64_t) RunPt);
    printf("Task 0: 0x%08X\r\n", (uint64_t) &ThreadsLL[0]);

    return 1;

}

// Launch OS with 1ms tick time
void OS_Launch(void) {
    enable_interrupt_controller();
    // Timer1_Init(TIME_1MS*1000, &TimerTick);
    enable_irq();
    __StartOS(RunPt);

    // Should not return here
    printf("ERROR: __StartOS returned to OS_Launch\r\n");
    while (1) {};
}

void OS_InString(char * s, uint32_t max) {
    UART_InString(s, max);
}

void OS_BoyKisser(void) {
    printf("\r\n");
    for (int i = 0; i < TEST_IMAGE_SIZE; i++) {
        printf("%s", test_image[i]);
    }
    printf("You like programming microcontrollers, don't you\r\n");
}

void OS_Init(void) {
    uart_init();
    init_printf(NULL, putc);
    irq_vector_init();

    uart_init();
    DisplayInit();

    // OS_BoyKisser();
}
