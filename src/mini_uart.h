// mini_uart.h
// https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson01/include/mini_uart.h

#ifndef	_MINI_UART_H
#define	_MINI_UART_H

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_string(char* str);

#endif  /*_MINI_UART_H */