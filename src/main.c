// main.c
// Brandon Zupan

#include "display.h"
#include "os.h"
#include "interpreter.h"
#include "mini_uart.h"
#include "printf.h"

void ThreadIdle(void) {
    // printf("Starting idle...\r\n");
    while (1) {
        // OS_Counter(0);
        // OS_Sleep(1000);
    }
}

void ThreadCount0(void) {
    while (1) {
        OS_Counter(0);
        OS_Sleep(100);
    }
}

void ThreadCount1(void) {
    while (1) {
        OS_Counter(1);
        OS_Sleep(200);
    }
}

void SleepSecondThread(void) {
    uint32_t count = 0;
    while (1) {
        printf("Count: %u\r\n", count);
        count++;
        OS_Sleep(1000);
    }
}

void SleepSecondThreadMain(void) {
    OS_Init();
    printf("Init...\r\n");
    OS_AddThread(&ThreadIdle, 1024, 1);
    OS_AddThread(&SleepSecondThread, 1024, 0);

    printf("Launching...\r\n");
    OS_Launch();
}

void InterpreterThreadMain(void) {
    OS_Init();
    OS_AddThread(&Interpreter, 1024, 0);
    
    OS_AddThread(&ThreadCount0, 1024, 1);
    OS_AddThread(&ThreadCount1, 1024, 1);

    OS_AddThread(&ThreadIdle, 1024, 2);

    // printf("Launching...\r\n");
    OS_Launch();
}

void kernel_main(void) {
    // SleepSecondThreadMain();
    InterpreterThreadMain();
}