// uart.c
// Brandon Zupan

#include <stdint.h>

#include "uart.h"
#include "mini_uart.h"

char UART_InChar(void) {
    return uart_recv();
}

void UART_OutChar(char c) {
    uart_send(c);
}


void UART_InString(char *bufPt, uint32_t max) {
int length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    if((character == BS) || (character == DEL)){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);   // clear output
        UART_OutChar(SP);   // blank to remove character
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}