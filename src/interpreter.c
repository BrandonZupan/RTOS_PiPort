// interpreter.c
// Brandon Zupan

#include "string.h"

#include "printf.h"
#include "os.h"

#define MAX_INPUT 128

extern int get_el(void);

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

void el(void) {
    int level = get_el();
	printf("EL%d\r\n", level);
}

void time(void) {
    uint32_t cur_time = OS_CurTime();
    printf("%u cycles\r\n", cur_time);
}

void timer1(void) {
    uint32_t next_time = OS_Timer1_Trigger();
    printf("%u cycles\r\n", next_time);
}

void switches(void) {
    uint64_t num_switches = OS_NumSwitches();
    printf("%u switches\r\n", num_switches);
}

/**
 * Interpreter
*/

void Interpreter(void) {
    printf("=== Interpreter === \r\n");
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
        } else
		
		if (strcmp (command, "el") == 0) {
			el();
		} else 

        if (strcmp(command, "time") == 0) {
            time();
        } else 

        if (strcmp(command, "timer1") == 0) {
            timer1();
        } else

        if (strcmp(command, "switches") == 0) {
            switches();
        }
    }
}
