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

void putc(void * p, char c) {
    // output both to UART and display
    Display_putc(c);
    uart_send(c);
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

void Scheduler(void) {
    
}

uint32_t OS_CurTime(void) {
    return Timer_CurTime();
}

uint32_t OS_Timer1_Trigger(void) {
    return Timer1_Trigger();
}

void OS_Init(void) {
    uart_init();
    init_printf(NULL, putc);
    irq_vector_init();
    enable_interrupt_controller();

    uart_init();
    DisplayInit();

    Timer1_Init(TIME_1MS, &Scheduler);

    // OS_BoyKisser();

    enable_irq();
}