#include "history.h"
#include <stdio.h>
#include <string.h>

#define HISTORY_SIZE 10
static char history[HISTORY_SIZE][100];
static int index = 0;

void add_to_history(const char* operation) {
    strncpy(history[index % HISTORY_SIZE], operation, 99);
    index++;
}

void display_history() {
    printf("\033[1;33m=== Calculation History ===\033[0m\n");
    int start = index > HISTORY_SIZE ? index - HISTORY_SIZE : 0;
    for (int i=start;i<index;i++)
        printf("%d: %s\n", i-start+1, history[i % HISTORY_SIZE]);
    printf("===========================\n");
}
