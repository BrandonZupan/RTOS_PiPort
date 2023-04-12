// interpreter.c
// Brandon Zupan

#include "string.h"

#include "printf.h"
#include "os.h"

#define MAX_INPUT 128

int FindSpace(char *string, int startIndex){
  int stringLength = strlen(string);
  char c;
  int index;
  for(index = startIndex; index < stringLength; index++){
    c = string[index];
    if(c == ' '){
      return index;
    }
  }
  //if no space is found, return the index of the end of the string
  return index;
}

/**
 * Commands
*/

void echo(char * input) {
    printf("%s\r\n", input);
}

void boykisser(void) {
    OS_BoyKisser();
}

/**
 * Interpreter
*/

void Interpreter(void) {
    char input [MAX_INPUT];
    
    while (1) {
        printf("BEvOS.Pi /> ");
        OS_InString(input, MAX_INPUT);
        printf("\r\n");

        // Do something
        // printf("%s\r\n", input);
        int space_index = FindSpace(input, 0);
        char command [MAX_INPUT];
        strncpy(command, input, space_index);
        command[space_index] = 0;

        if (strcmp(command, "echo") == 0) {
            echo(&input[space_index + 1]);
        } else 

        if (strcmp(command, "boykisser") == 0) {
            boykisser();
        }
    }
}
