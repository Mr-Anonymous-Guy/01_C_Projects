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
    system("cls");
#else
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

    strftime(buffer, max_len, "%Y-%m-%d %H:%M:%S", timeinfo);
}

int read_int(int *val) {
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    input[strcspn(input, "\n")] = '\0';

    char extra;
    if (sscanf(input, "%d %c", val, &extra) == 1) {
        return 1;
    }
    return 0;
}

int read_double(double *val) {
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    input[strcspn(input, "\n")] = '\0';

    char extra;
    if (sscanf(input, "%lf %c", val, &extra) == 1) {
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
    temp[strcspn(temp, "\n")] = '\0';

    char *start = temp;
    while (*start == ' ' || *start == '\t' || *start == '\r') {
        start++;
    }

    int len = strlen(start);
    while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t' || start[len - 1] == '\r')) {
        start[len - 1] = '\0';
        len--;
    }

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
        sleep_ms(200);
        printf(".");
        fflush(stdout);
    }
    sleep_ms(200);
    printf("\n");
}

void press_enter_to_continue(void) {
    printf("\nPress [Enter] to continue...");
    fflush(stdout);
    char ch;
    while (scanf("%c", &ch) == 1 && ch != '\n');
}
