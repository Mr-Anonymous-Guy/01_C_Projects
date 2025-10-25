#include "patterns.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice, rows, mode_choice;
    char fill;
    FILE *history;

    history = fopen("history/output.txt", "a");
    if (!history) {
        perror("Cannot open history file");
        return 1;
    }

    while (1) {
        printf("\n=== Pattern Printer ===\n");
        printf("1. Pyramid\n2. Inverted Pyramid\n3. Number Pyramid\n4. Pascal Triangle\n");
        printf("5. Diamond\n6. Hollow Diamond\n7. Butterfly\n8. Exit\n");
        printf("Enter choice (1-8): ");
        if (scanf("%d", &choice) != 1) break;
        if (choice == 8) break;

        printf("Enter number of rows: ");
        if (scanf("%d", &rows) != 1) break;

        PatternOptions opt_screen, opt_file;
        opt_screen.rows = rows;
        opt_file.rows = rows;
        opt_screen.type = choice - 1;
        opt_file.type = choice - 1;

        // Ask print mode (except Pascal)
        if (choice != 4) {
            printf("Choose print mode:\n1. Character (\"@\", \"#\", \"*\")\n2. Numbers\n3. Alphabet (a,b,c...)\nEnter: ");
            if (scanf("%d", &mode_choice) != 1) break;
        } else {
            mode_choice = 2; // Pascal always numbers
        }

        switch(mode_choice) {
            case 1:
                opt_screen.mode = PRINT_CHAR;
                opt_file.mode = PRINT_CHAR;
                printf("Enter character to print: ");
                scanf(" %c", &fill);
                opt_screen.fill = fill;
                opt_file.fill = fill;
                break;
            case 2:
                opt_screen.mode = PRINT_NUMBER;
                opt_file.mode = PRINT_NUMBER;
                opt_screen.fill = '1';
                opt_file.fill = '1';
                break;
            case 3:
                opt_screen.mode = PRINT_ALPHA;
                opt_file.mode = PRINT_ALPHA;
                opt_screen.fill = 'a';
                opt_file.fill = 'a';
                break;
            default:
                printf("Invalid mode, using '*'\n");
                opt_screen.mode = PRINT_CHAR;
                opt_file.mode = PRINT_CHAR;
                opt_screen.fill = '*';
                opt_file.fill = '*';
        }

        // Print to screen
        opt_screen.out = stdout;
        printf("\n--- Pattern ---\n");
        print_pattern(&opt_screen);

        // Print to file
        opt_file.out = history;
        print_pattern(&opt_file);
        fprintf(history, "\n--- End of Pattern ---\n");
        fflush(history);
        printf("History stored.\n");
    }

    fclose(history);
    printf("Exiting program.\n");
    return 0;
}
