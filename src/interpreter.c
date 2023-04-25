// interpreter.c
// Brandon Zupan

#include "string.h"
#include <stdint.h>
#include <stddef.h>

#include "printf.h"
#include "os.h"
#include "irq.h"

#define MAX_INPUT 128

Sema4_t semaphore_demo;

extern int get_el(void);

int FindSpace(char *string, int startIndex){
  int stringLength = strlen(string);
  char c;
  int index;
  for(index = startIndex; index < stringLength; index++){
    c = string[index];
    if(c == ' '){
      return index;
    }
  }
  //if no space is found, return the index of the end of the string
  return index;
}

void SemaphoreWait(void) {
    // Wait on a semaphore then print a message
    printf("WAITER: Waiting on Sema4\r\n");
    OS_bWait(&semaphore_demo);
    printf("WAITER: Finished waiting\r\n");

    OS_Kill();
}

void SemaphoreSignal(void) {
    printf("SIGNALER: Sleeping for 3 seconds\r\n");
    OS_Sleep(3000);

    printf("SIGNALER: Signaling\r\n");
    OS_bSignal(&semaphore_demo);

    OS_Kill();
}

/**
 * Commands
*/

void echo(char * input) {
    printf("%s\r\n", input);
}

void boykisser(void) {
    OS_BoyKisser();
}

void el(void) {
    int level = get_el();
	printf("EL%d\r\n", level);
}

void time(void) {
    uint32_t cur_time = OS_CurTime();
    printf("%u cycles\r\n", cur_time);
}

void timer1(void) {
    uint32_t next_time = OS_Timer1_Trigger();
    printf("%u cycles\r\n", next_time);
}

void switches(void) {
    uint64_t num_switches = OS_NumSwitches();
    printf("%u switches\r\n", num_switches);
}

void null(void) {
    uint8_t *p = NULL;
    printf("0x%02X%02X%02X%02X\r\n", ((const uint8_t *)&p)[0], ((const uint8_t *)&p)[1], ((const uint8_t *)&p)[2], ((const uint8_t *)&p)[3]);
}

// void counter(uint32_t counter) {
//     uint64_t counts = OS_GetCounter(counter);
//     printf("%u\r\n", counts);
// }

void counters(void) {
    uint64_t count0 = OS_GetCounter(0);
    uint64_t count1 = OS_GetCounter(1);
    printf("Counter 0: %u\r\n", count0);
    printf("Counter 1: %u\r\n", count1);
}

void sleep(uint32_t ms) {
    printf("Sleeping for %u ms\r\n", ms);
    OS_Sleep(ms);
}

void semaphore(void) {
    // Create 2 threads, one waits on the other
    OS_DisableScheduler();

    OS_InitSemaphore(&semaphore_demo, 0);

    OS_AddThread(&SemaphoreWait, 1024, 0);
    OS_AddThread(&SemaphoreSignal, 1024, 1);

    OS_EnableScheduler();
    OS_Sleep(6000);
}

/**
 * Interpreter
*/

void Interpreter(void) {
    printf("=== Interpreter === \r\n");
    char input [MAX_INPUT];
    
    while (1) {
        printf("BEvOS.Pi /> ");
        // disable_irq();
        OS_InString(input, MAX_INPUT);
        // enable_irq();
        printf("\r\n");

        // Do something
        // printf("Received: %s\r\n", input);
        int space_index = FindSpace(input, 0);
        char command [MAX_INPUT];
        strncpy(command, input, space_index);
        command[space_index] = 0;

        if (strcmp(command, "echo") == 0) {
            echo(&input[space_index + 1]);
        } else 

        if (strcmp(command, "boykisser") == 0) {
            boykisser();
        } else
		
		if (strcmp (command, "el") == 0) {
			el();
		} else 

        if (strcmp(command, "time") == 0) {
            time();
        } else 

        if (strcmp(command, "timer1") == 0) {
            timer1();
        } else

        if (strcmp(command, "switches") == 0) {
            switches();
        } else 

        if (strcmp(command, "counters") == 0) {
            counters();
        } else 

        if (strcmp(command, "sleep") == 0) {
            sleep(5000);
        } else 

        if (strcmp(command, "null") == 0) {
            null();
        } else 

        if (strcmp(command, "semaphore") == 0) {
            semaphore();
        }
    }
}
