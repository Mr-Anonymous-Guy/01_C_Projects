#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clear_screen(void) {
#ifdef _WIN32
    // Windows clear screen execution
    system("cls");
#else
    // UNIX ANSI escape sequence for clear screen and home cursor
    printf("\033[H\033[J");
    fflush(stdout);
#endif
}

void print_separator(char ch, int len) {
    for (int i = 0; i < len; i++) {
        putchar(ch);
    }
    putchar('\n');
}

void get_timestamp(char *buffer, int max_len) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format: YYYY-MM-DD HH:MM:SS
    strftime(buffer, max_len, "%Y-%m-%d %H:%M:%S", timeinfo);
}

int read_int(int *val) {
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    // Remove newline
    input[strcspn(input, "\n")] = '\0';

    // Verify input contains only digits and optional leading sign
    char extra;
    if (sscanf(input, "%d %c", val, &extra) == 1) {
        return 1;
    }
    return 0;
}

int read_float(float *val) {
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    // Remove newline
    input[strcspn(input, "\n")] = '\0';

    // Verify float format
    char extra;
    if (sscanf(input, "%f %c", val, &extra) == 1) {
        return 1;
    }
    return 0;
}

void read_string(char *buffer, int max_len) {
    char temp[512];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    // Remove trailing newline character
    temp[strcspn(temp, "\n")] = '\0';

    // Trim leading whitespace
    char *start = temp;
    while (*start == ' ' || *start == '\t' || *start == '\r') {
        start++;
    }

    // Trim trailing whitespace
    int len = strlen(start);
    while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t' || start[len - 1] == '\r')) {
        start[len - 1] = '\0';
        len--;
    }

    // Copy to destination buffer up to max_len - 1 characters
    strncpy(buffer, start, max_len - 1);
    buffer[max_len - 1] = '\0';
}

void sleep_ms(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void show_loading_effect(const char *message) {
    printf("%s", message);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        sleep_ms(250);
        printf(".");
        fflush(stdout);
    }
    sleep_ms(250);
    printf("\n");
}

void press_enter_to_continue(void) {
    printf("\nPress [Enter] to continue...");
    fflush(stdout);
    char ch;
    while (scanf("%c", &ch) == 1 && ch != '\n');
}
