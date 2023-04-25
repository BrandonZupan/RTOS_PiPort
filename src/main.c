// main.c
// Brandon Zupan

#include "display.h"
#include "os.h"
#include "interpreter.h"
#include "mini_uart.h"
#include "printf.h"

Sema4_t TestSema4;

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

void HeartbeatThread(void) {
    uint64_t count = 0;
    while (1) {
        printf("Heartbeat %d\r\n", count);
        count++;
        OS_Sleep(5000);
    }
}

void Sema4WaiterThread(void) {
    // Wait on a semaphore then print a message
    printf("WAITER: Waiting on Sema4\r\n");
    OS_bWait(&TestSema4);
    printf("WAITER: Finished waiting\r\n");

    OS_Kill();
    while (1) {
        printf("WAITER: failed to kill\r\n");
    };
}

void Sema4SignalerThread(void) {
    printf("SIGNALER: Sleeping for 3 seconds\r\n");
    OS_Sleep(3000);

    printf("SIGNALER: Signaling\r\n");
    OS_bSignal(&TestSema4);

    OS_Kill();
    while(1) {
        printf("SIGNALER: failed to kill\r\n");
    };
}

void SleepSecondThreadMain(void) {
    OS_Init();
    printf("Init...\r\n");
    OS_AddThread(&ThreadIdle, 1024, 1);
    OS_AddThread(&SleepSecondThread, 1024, 0);

    printf("Launching...\r\n");
    OS_Launch();
}

void Sema4TestMain(void) {
    OS_Init();
    OS_InitSemaphore(&TestSema4, 0);
    printf("=== Semaphore Test Main ===\r\n");

    OS_AddThread(&ThreadIdle, 1024, 3);
    OS_AddThread(&Sema4WaiterThread, 1024, 1);
    OS_AddThread(&Sema4SignalerThread, 1024, 2);
    OS_AddThread(&HeartbeatThread, 1024, 0);

    OS_Launch();
}

void InterpreterThreadMain(void) {
    OS_Init();
    OS_AddThread(&Interpreter, 1024, 3);
    
    OS_AddThread(&ThreadCount0, 1024, 4);
    OS_AddThread(&ThreadCount1, 1024, 4);

    OS_AddThread(&ThreadIdle, 1024, 5);

    // printf("Launching...\r\n");
    OS_Launch();
}

void kernel_main(void) {
    // SleepSecondThreadMain();
    // Sema4TestMain();
    InterpreterThreadMain();
}