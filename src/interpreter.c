// interpreter.c
// Brandon Zupan

#include "printf.h"
#include "os.h"

#define MAX_INPUT 128

void Interpreter(void) {
    char input [MAX_INPUT];
    
    while (1) {
        printf("BEvOS /> ");
        OS_GetString(input, MAX_INPUT);

        // Do something
    }

}