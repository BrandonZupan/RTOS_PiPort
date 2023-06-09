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

#define MAX_THREADS     10
#define NUM_COUNTERS    6
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
    // Display_putc(c);
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
        // disable_irq();
        num_switches++;
        TCB_t * next_task = FindNextTask();
        // printf("Next task ID: %d\r\n", next_task->id);
        SwitchTask(next_task);
        // enable_irq();    // done in __SwitchTask()
    } else {
        // printf("SCHEDULER: Scheduler disabled...\r\n");
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

    // call every 100ms (or something, idk)
    if (count >= 100) {
        // printf("TIMERTICK: Timer calling scheduler...\r\n");
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

    // disable_irq();
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
    ThreadsLL[first_free_thread].initial_sp = ThreadsLL[first_free_thread].sp;
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

    // enable_irq();

    // printf("RunPt: 0x%08X\r\n", (uint64_t) RunPt);
    // printf("Task %u: 0x%08X\r\n", first_free_thread, (uint64_t) &ThreadsLL[first_free_thread]);

    // printf("New Thread ID: %u", ThreadsLL[first_free_thread].id);
    // printf("\tSP: 0x%08X\r\n", ThreadsLL[first_free_thread].sp);

    return 1;

}

// Launch OS with 1ms tick time
void OS_Launch(void) {

    // Emulate what example Pi OS does
    Timer1_Init(TIME_1MS, &TimerTick);
    OS_EnableScheduler();
    enable_irq();

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

void OS_PrintTCB(TCB_t * tcb) {
    printf("TCB ID: %u\r\n", tcb->id);
    printf("\tx19: 0x%08X\r\n", tcb->x19);
    printf("\tx20: 0x%08X\r\n", tcb->x20);
    printf("\tx21: 0x%08X\r\n", tcb->x21);
    printf("\tx22: 0x%08X\r\n", tcb->x22);
    printf("\tx23: 0x%08X\r\n", tcb->x23);
    printf("\tx24: 0x%08X\r\n", tcb->x24);
    printf("\tx25: 0x%08X\r\n", tcb->x25);
    printf("\tx26: 0x%08X\r\n", tcb->x26);
    printf("\tx27: 0x%08X\r\n", tcb->x27);
    printf("\tx28: 0x%08X\r\n", tcb->x28);
    printf("\txfp: 0x%08X\r\n", tcb->fp);
    printf("\tSP: 0x%08X\r\n", tcb->sp);
    printf("\tPC: 0x%08X\r\n", tcb->pc);
}

void OS_PrintStackTrace(TCB_t * tcb) {
    printf("Stack Trace\r\n");
    uint64_t * sp = (uint64_t *) tcb->sp;
    uint64_t i = 0;
    while (&sp[i] <= (uint64_t *) tcb->initial_sp) {
        printf("\t0x%08X: 0x%08X\r\n", &sp[i], sp[i]);
        i++;
    }
}

void OS_PrintRunPt(void) {
    OS_PrintTCB(RunPt);
    OS_PrintStackTrace(RunPt);
}

void OS_InitSemaphore(Sema4_t *sema_pt, int32_t value) {
    // OS_DisableScheduler();
    sema_pt->value = value;
    // OS_EnableScheduler();
}

void OS_bWait(Sema4_t *sema_pt) {
    OS_DisableScheduler();
    // printf("Semaphore value: %d\r\n", sema_pt->value);
    while (sema_pt->value == 0) {
        RunPt->block_pt = sema_pt;
        OS_EnableScheduler();
        Scheduler();
        sema_pt->value = 0;
        enable_irq();
        return;
    }
    sema_pt->value = 0;
    enable_irq();
    OS_EnableScheduler();
}

void OS_bSignal(Sema4_t *sema_pt) {
    OS_DisableScheduler();
    if (sema_pt->value <= 0) {
        // iterate through all threads and find the one that is blocked
        for (int i = 0; i < MAX_THREADS; i++) {
            if (ThreadsLL[i].is_valid && (ThreadsLL[i].block_pt == sema_pt)) {
                ThreadsLL[i].block_pt = NULL;
                // printf("No longer blocking thread %d\r\n", ThreadsLL[i].id);
                enable_irq();
            }
        }
    }

    sema_pt->value = 1;
    OS_EnableScheduler();
}

void OS_Kill(void) {
    OS_DisableScheduler();
    RunPt->is_valid = 0;

    // check if this is the only thread with its priority
    if (RunPt->next == RunPt) {
        PriorityPointers[RunPt->prio] = NULL;
    }
    else {
        // remove it from linked list
        RunPt->prev->next = RunPt->next;
        RunPt->next->prev = RunPt->prev;
    }

    OS_EnableScheduler();
    // printf("KILL: Killed\r\n");
    while (1) {};   // wait for scheduler to move on
    // OS_Sleep(1000);
    // Scheduler();
}

void OS_Init(void) {
    uart_init();
    init_printf(NULL, putc);
    irq_vector_init();
    enable_interrupt_controller();
    InitialTaskInit();

    uart_init();
    // DisplayInit();
    // OS_BoyKisser();
}
