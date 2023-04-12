// uart.h
// Brandon Zupan

#ifndef _UART_H
#define _UART_H

#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

char UART_InChar(void);

void UART_OutChar(char c);

void UART_InString(char *bufPt, uint32_t max);


#endif  /*_UART_H*/