// os.c
// Brandon Zupan

#include <stdint.h>
#include <stddef.h>

#include "printf.h"
#include "display.h"
#include "furry.h"
#include "os.h"
#include "mini_uart.h"

void putc(void * p, char c) {
    // output both to UART and display
    Display_putc(c);
    uart_send(c);
}

void OS_GetString(char * s, uint32_t size) {
    // TODO: Get input from UART
}

void print_test_image(void) {
    for (int i = 0; i < TEST_IMAGE_SIZE; i++) {
        printf("%s", test_image[i]);
    }
}

void OS_Init(void) {
    init_printf(NULL, putc);

    uart_init();
    DisplayInit();

    print_test_image();
    printf("You like programming microcontrollers, don't you\r\n");
}