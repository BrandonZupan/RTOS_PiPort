// display.c
// Brandon Zupan

#include <stdint.h>

#include "lfb.h"

const uint32_t X_RES = 1024;
const uint32_t Y_RES = 768;

uint32_t x = 80;
uint32_t y = 80;

int DisplayInit(void) {
    // Initializes HDMI display
    lfb_init();
    return 1;
}

void DisplayPrintS(const char* s) {
    // print until reaching null terminator
    lfb_print(80, y, (char *) s);
    y = (y + 8) % Y_RES;
}