#include "operations.h"
#include <stdio.h>

float add(float a, float b) { return a + b; }
float subtract(float a, float b) { return a - b; }
float multiply(float a, float b) { return a * b; }
float divide(float a, float b) {
    if (b == 0) {
        printf("\033[1;31mError: Division by zero!\033[0m\n");
        return 0;
    }
    return a / b;
}
float percentage(float value, float percent) { return (value * percent)/100; }
