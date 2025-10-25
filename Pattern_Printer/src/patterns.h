#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdio.h>

typedef enum {
    P_PYRAMID,
    P_INVERTED,
    P_NUMBER_PYRAMID,
    P_PASCAL,
    P_DIAMOND,
    P_HOLLOW_DIAMOND,
    P_BUTTERFLY
} PatternType;

typedef enum {
    PRINT_CHAR,
    PRINT_NUMBER,
    PRINT_ALPHA
} PrintMode;

typedef struct {
    PatternType type;
    int rows;
    char fill;
    PrintMode mode;
    FILE *out;
} PatternOptions;

// Main function to print any pattern
int print_pattern(const PatternOptions *opt);

#endif
