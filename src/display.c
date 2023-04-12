// display.c
// Brandon Zupan

#include <stdint.h>

#include "lfb.h"

const uint32_t X_RES = 1024;
const uint32_t Y_RES = 768;
const uint32_t X_CHAR = 8;
const uint32_t Y_CHAR = 16;

uint32_t x;
uint32_t y;

int DisplayInit(void) {
    x = 0;
    y = 0;

    // Initializes HDMI display
    lfb_init();
    return 0;
}

int Display_putc(char c) {
    if (c == '\n') {
        y = (y + Y_CHAR) % Y_RES;
        x = 0;
    } else if (c == '\r') {
        x = 0;
    } else {
        lfb_putc(x, y, c);
    }
}

void DisplayPrintS(const char* s) {
    // print until reaching null terminator
    lfb_print(80, y, (char *) s);
    y = (y + 8) % Y_RES;
}