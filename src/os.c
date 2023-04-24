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
#include "mm.h"

// #define ROUND_ROBIN
#define PRIORITY_SCHEDULER

#define MAX_THREADS     6
#define NUM_COUNTERS    10
#define PRIORITY_LEVELS 6

extern void __SwitchTask(TCB_t * prev, TCB_t * next);
extern void __StartOS(TCB_t * run_pt);
extern void ret_from_fork(void);

TCB_t ThreadsLL[MAX_THREADS];
TCB_t * PriorityPointers[PRIORITY_LEVELS];

TCB_t * RunPt;
TCB_t * InitialTask;

uint64_t num_switches = 0;
uint8_t is_running = 0;     // set to 1 after first context switch
uint8_t is_scheduler_enabled = 0;

uint64_t Counters [NUM_COUNTERS];   // performance counters

void putc(void * p, char c) {
    // output both to UART and display
    Display_putc(c);
    uart_send(c);
}

int findFirstFreeThread(void){
  for(int i = 1; i < MAX_THREADS; i++){
    if(!ThreadsLL[i].is_valid) {
        return i;
    }
  }
  return MAX_THREADS;
}

TCB_t * FindNextTaskRoundRobin(void) {
    TCB_t * current = RunPt;

    // find next task that is not sleeping and not blocked
    do {
        current = current->next;
    } while ((current->sleep != 0) || (current->block_pt != NULL));

    return current;
}

TCB_t * FindNextTaskPriority(void) {
    for (int i = 0; i < PRIORITY_LEVELS; i++) {
        if (PriorityPointers[i] != NULL) {
            TCB_t * temp_ptr = PriorityPointers[i];
            // try to find entry that is alive
            while (1) {
                // if we find something, return it
                if (temp_ptr->sleep == 0 && temp_ptr->block_pt == NULL) {
                    PriorityPointers[i] = temp_ptr->next;   // round robin throughout priority level
                    return temp_ptr;
                }
                else {
                    // check next entry at level
                    temp_ptr = temp_ptr->next;
                    if (temp_ptr == PriorityPointers[i]) {
                        // try next level
                        break;
                    }
                }
            }
        }
    }

    // If we are here then it found no threads
    return NULL;
}

TCB_t * FindNextTask(void) {
#ifdef ROUND_ROBIN
    return FindNextTaskRoundRobin();
#endif

#ifdef PRIORITY_SCHEDULER
    return FindNextTaskPriority();
#endif


    return NULL;
}

void SwitchTask(TCB_t * next) {
    // if (next == RunPt) {
    //     enable_irq();
    //     return;
    // }

    // 
    TCB_t * prev;
    if (is_running == 0) {
        prev = InitialTask;
        is_running = 1;
    }
    else {
        prev = RunPt;
    }

    RunPt = next;
    if (prev->id != next->id) {
        // printf("Calling __SwitchTask... prev ID: %d, next ID: %d\r\n", prev->id, next->id);
        // printf("Next PC: 0x%08X\r\n", next->pc);
    }
    // static int count = 0;
    // if (count == 5) {
    //     disable_irq();
    //     while (1) {};
    // }
    // count++;
    // printf("Will stack pointer: 0x%016X\r\n", next->sp);
    __SwitchTask(prev, next);
}

// Called every timer tick, so prob 1ms
// Gets next task and runs it
void Scheduler(void) {
    if (is_scheduler_enabled) {
        disable_irq();
        num_switches++;
        TCB_t * next_task = FindNextTask();
        // printf("Next task ID: %d\r\n", next_task->id);
        SwitchTask(next_task);
        // enable_irq();    // done in __SwitchTask()
    }
}

// Called around every MS
void SleepDecriment(void) {
    // Iterate through TCB array and decriment sleep times
    for (int i = 0; i < MAX_THREADS; i++) {
        if (ThreadsLL[i].is_valid) {
            if (ThreadsLL[i].sleep > 0) {
                ThreadsLL[i].sleep--;
                if (ThreadsLL[i].sleep == 0) {
                    // printf("Thread %u no longer sleeping\r\n", ThreadsLL[i].id);
                }
            }
        }
    }
}

// Called every timer tick
void TimerTick(void) {
    static uint32_t count = 0;

    // printf("TimerTick...\r\n");
    SleepDecriment();

    // call every 10ms (or something, idk)
    if (count >= 100) {
        Scheduler();
        count = 0;
    }
    else {
        count++;
    }
}

void InitialTaskInit(void) {
    // Set initial task
    ThreadsLL[0].is_valid = 1;      // keeps findFirstFreeThread from finding it
    ThreadsLL[0].id = 69420;
    InitialTask = &ThreadsLL[0];
    is_running = 0;
}

void Debug_ReturnFromFormMsg(uint64_t jump_addr) {
    // printf("Return from fork... Jumping to 0x%08X\r\n", jump_addr);
    // for (uint64_t i = 0; i < TIME_1MS * 1000; i++) {
    //     //hang out here
    // }
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
    // ThreadsLL[first_free_thread].sp = (uint64_t) &ThreadsLL->stack[0];
    ThreadsLL[first_free_thread].sp = get_free_page();
    // printf("Stack Pointer: 0x%016X\r\n", ThreadsLL[first_free_thread].sp);

    ThreadsLL[first_free_thread].sleep = 0;
    ThreadsLL[first_free_thread].id = next_id++;
    ThreadsLL[first_free_thread].prio = priority;
    ThreadsLL[first_free_thread].block_pt = NULL;

    if (RunPt == NULL) {
        // this is the first thread
        RunPt = &ThreadsLL[first_free_thread];
        RunPt->next = RunPt;
        RunPt->prev = RunPt;

        #ifdef PRIORITY_SCHEDULER
        // add it to priority list
        PriorityPointers[priority] = RunPt;
        #endif
    } else {
        #ifdef ROUND_ROBIN
        TCB_t *temp_next = RunPt->next;

        RunPt->next = &ThreadsLL[first_free_thread];    // run it next
        ThreadsLL[first_free_thread].next = temp_next;
        ThreadsLL[first_free_thread].prev = RunPt;
        temp_next->prev = &ThreadsLL[first_free_thread];
        #endif

        #ifdef PRIORITY_SCHEDULER
        // Insert it into its priority level
        if (PriorityPointers[priority] != NULL) {
            // Add to beginning
            TCB_t * old_first = PriorityPointers[priority];
            TCB_t * last = old_first->prev;
            PriorityPointers[priority] = &ThreadsLL[first_free_thread];

            // old first is after new entry
            PriorityPointers[priority]->next = old_first;
            old_first->prev = PriorityPointers[priority];

            // update last so it points to new first
            last->next = PriorityPointers[priority];
            PriorityPointers[priority]->prev = last;
        }
        else {
            PriorityPointers[priority] = &ThreadsLL[first_free_thread];
            PriorityPointers[priority]->next = PriorityPointers[priority];
            PriorityPointers[priority]->prev = PriorityPointers[priority];
        }
        #endif
    }

    enable_irq();

    // printf("RunPt: 0x%08X\r\n", (uint64_t) RunPt);
    // printf("Task %u: 0x%08X\r\n", first_free_thread, (uint64_t) &ThreadsLL[first_free_thread]);

    return 1;

}

// Launch OS with 1ms tick time
void OS_Launch(void) {

    // Emulate what example Pi OS does
    Timer1_Init(TIME_1MS, &TimerTick);
    OS_EnableScheduler();

    while (1) {
        // printf("OS Launch loop\r\n");
        Scheduler();
    }

    // enable_interrupt_controller();
    // // Timer1_Init(TIME_1MS*1000, &TimerTick);
    // enable_irq();
    // __StartOS(RunPt);

    // // Should not return here
    // printf("ERROR: __StartOS returned to OS_Launch\r\n");
    // while (1) {};
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

void OS_Counter(uint32_t counter) {
    // OS_DisableScheduler();
    if (counter < NUM_COUNTERS) {
        Counters[counter]++;
    }
    // OS_EnableScheduler();
}

uint64_t OS_GetCounter(uint32_t counter) {
    uint64_t result = 0;
    // OS_DisableScheduler();

    if (counter < NUM_COUNTERS) {
        result = Counters[counter];

    }

    // OS_EnableScheduler();
    return result;
}

void OS_DisableScheduler(void) {
    is_scheduler_enabled = 0;
}

void OS_EnableScheduler(void) {
    is_scheduler_enabled = 1;
}

void OS_Init(void) {
    uart_init();
    init_printf(NULL, putc);
    irq_vector_init();
    enable_interrupt_controller();
    InitialTaskInit();

    uart_init();
    DisplayInit();
    // OS_BoyKisser();
}
