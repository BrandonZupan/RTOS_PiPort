// main.c
// Brandon Zupan

#include "display.h"
#include "os.h"
#include "interpreter.h"
#include "mini_uart.h"

void kernel_main(void)
{
    // uart_init();
    // uart_send_string("You like programming microcontrollers, don't you\r\n");

    // while (1) {
    //     uart_send(uart_recv());
    // }

    OS_Init();

    Interpreter();

}