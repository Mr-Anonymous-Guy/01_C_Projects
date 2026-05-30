#include <stdio.h>
#include "menu.h"
#include "utils.h"

void print_header(const char *title) {
    clear_screen();
    printf("============================================================\n");
    printf("                   ATM MANAGEMENT SYSTEM                    \n");
    printf("============================================================\n");
    if (title && title[0] != '\0') {
        printf(" >> %s\n", title);
        print_separator('-', 60);
    }
}

void display_welcome_menu(void) {
    print_header("");
    printf("  [1] Create New Account\n");
    printf("  [2]  Login to Account\n");
    printf("  [3]  Exit System\n");
    print_separator('=', 60);
    printf(" Please select an option (1-3): ");
}

void display_main_menu(const char *name, int account_num) {
    char title[128];
    snprintf(title, sizeof(title), "Welcome, %s | Account: %06d", name, account_num);
    print_header(title);
    printf("  [1]  Deposit Funds\n");
    printf("  [2]  Withdraw Funds\n");
    printf("  [3]  Check Balance\n");
    printf("  [4]  View Transaction Ledger\n");
    printf("  [5]  Logout\n");
    print_separator('=', 60);
    printf(" Please select an option (1-5): ");
}
