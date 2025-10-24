#include "utils.h"
#include <stdio.h>
#include <stdlib.h>  // for system()

float get_number(const char* prompt) {
    float num;
    printf("\033[1;36m%s\033[0m", prompt); // Cyan prompt
    while (scanf("%f", &num) != 1) {
        while(getchar() != '\n'); // clear invalid input
        printf("\033[1;31mInvalid input. Try again: \033[0m"); // Red error
    }
    return num;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_result(float result) {
    printf("\033[1;32mResult: %.2f\033[0m\n", result); // Green
}

void print_header(const char* text) {
    printf("\n\033[1;34m=== %s ===\033[0m\n", text); // Blue
}
