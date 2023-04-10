// main.c
// Brandon Zupan

#include "display.h"
#include "furry.h"

void kernel_main(void)
{
    // uart_init();
    // uart_send_string("Hello, world!\r\n");
    DisplayInit();

    // DisplayPrintS("Pee pee poo poo\n");

    for (int i = 0; i < TEST_IMAGE_SIZE; i++) {
        DisplayPrintS(test_image[i]);
    }

    // while (1) {
    //     // uart_send(uart_recv());
    // }
}