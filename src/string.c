// string.c
// Brandon Zupan

#include <stdint.h>

#include "string.h"

void strncpy(char *dst, const char *src, uint32_t n) {
    // copy first N characters from src to dst
    // If null terminator is reached, pad with 0s
    uint8_t zero_pad = 0;
    for (uint32_t i = 0; i < n; i++) {
        if (zero_pad) {
            dst[i] = 0;
        } else {
            dst[i] = src[i];
            if (src[i] == 0) {
                zero_pad = 1;
            }
        }
    }
}

uint32_t strlen(const char *src) {
    // get number of characters in string
    uint32_t result = 0;
    while (src[result] != 0) {
        result++;
    }

    return result;
}

// returns bool, 0 if equal, 1 if not equal
uint8_t strcmp(const char *str1, const char *str2) {
    uint32_t i = 0;
    while (str1[i] == str2[i]) {
        if (str1[i] == 0) {
            // we reached end
            return 0;
        }
        i++;
    }
    // weren't equal
    return 1;
}


