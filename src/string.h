// string.h
// Brandon Zupan

#ifndef _STRING_H
#define _STRING_H

#include <stdint.h>

void strncpy(char *dst, const char *src, uint32_t n);
uint32_t strlen(const char *src);
uint8_t strcmp(const char *str1, const char *str2);

#endif  /*_STRING_H*/