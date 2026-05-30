#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bank.h"
#include "account.h"
#include "transaction.h"
#include "file_handler.h"
#include "logger.h"
#include "menu.h"
#include "utils.h"
#include "auth.h"

static void handle_deposit(Account *active_acc) {
    print_header("DEPOSIT FUNDS");
    double amount = 0.0;
    printf(" Enter amount to deposit ($): ");
    if (!read_double(&amount) || amount <= 0.0) {
        printf("\n[ERROR] Deposit amount must be a positive number.\n");
        return;
    }

    show_loading_effect(" Processing deposit");
    if (deposit(active_acc, amount)) {
        printf("\n[SUCCESS] Deposited $%.2f. New Balance: $%.2f\n", amount, active_acc->balance);
    } else {
        printf("\n[ERROR] Deposit operation failed.\n");
    }
}

static void handle_withdraw(Account *active_acc) {
    print_header("WITHDRAW FUNDS");
    double amount = 0.0;
    printf(" Available Balance: $%.2f\n", active_acc->balance);
    printf(" Enter amount to withdraw ($): ");
    if (!read_double(&amount) || amount <= 0.0) {
        printf("\n[ERROR] Withdrawal amount must be a positive number.\n");
        return;
    }

    show_loading_effect(" Processing withdrawal");
    if (withdraw(active_acc, amount)) {
        printf("\n[SUCCESS] Withdrew $%.2f. Remaining Balance: $%.2f\n", amount, active_acc->balance);
    } else {
        if (active_acc->balance < amount) {
            printf("\n[ERROR] Insufficient funds. Transaction cancelled.\n");
        } else {
            printf("\n[ERROR] Withdrawal operation failed.\n");
        }
    }
}

static void handle_transfer(Account *active_acc) {
    print_header("TRANSFER FUNDS");
    printf(" Available Balance: $%.2f\n", active_acc->balance);
    printf(" Enter recipient's 6-digit Account Number: ");

    char dest_str[16];
    read_string(dest_str, sizeof(dest_str));

    if (strlen(dest_str) != 6) {
        printf("\n[ERROR] Account Number must be exactly 6 digits.\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)dest_str[i])) {
            printf("\n[ERROR] Account Number must contain only digits.\n");
            return;
        }
    }
    int dest_num = atoi(dest_str);

    if (dest_num == active_acc->accountNumber) {
        printf("\n[ERROR] You cannot transfer funds to your own account.\n");
        return;
    }

    Account receiver;
    if (!load_account(dest_num, &receiver)) {
        printf("\n[ERROR] Recipient account %06d does not exist.\n", dest_num);
        return;
    }

    if (receiver.isFrozen) {
        printf("\n[ERROR] Recipient account %06d is currently frozen and cannot receive funds.\n", dest_num);
        return;
    }

    double amount = 0.0;
    printf(" Enter transfer amount ($): ");
    if (!read_double(&amount) || amount <= 0.0) {
        printf("\n[ERROR] Transfer amount must be a positive number.\n");
        return;
    }

    printf("\n Confirm transfer of $%.2f to %s (%06d)? (Y/N): ", amount, receiver.name, receiver.accountNumber);
    char confirm[10];
    read_string(confirm, sizeof(confirm));
    if (strcmp(confirm, "Y") != 0 && strcmp(confirm, "y") != 0) {
        printf("\n[INFO] Transfer cancelled.\n");
        return;
    }

    show_loading_effect(" Processing transfer");
    if (transfer(active_acc, &receiver, amount)) {
        printf("\n[SUCCESS] Transferred $%.2f to %s. New Balance: $%.2f\n", amount, receiver.name, active_acc->balance);
    } else {
        if (active_acc->balance < amount) {
            printf("\n[ERROR] Insufficient funds.\n");
        } else {
            printf("\n[ERROR] Transfer operation failed.\n");
        }
    }
}

static void handle_change_pin(Account *active_acc) {
    print_header("CHANGE SECURITY PIN");
    int old_pin = 0;
    printf(" Enter current 4-Digit PIN: ");
    char pin_str[16];
    read_string(pin_str, sizeof(pin_str));

    if (strlen(pin_str) != 4) {
        printf("\n[ERROR] PIN must be exactly 4 digits.\n");
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)pin_str[i])) {
            printf("\n[ERROR] PIN must contain only digits.\n");
            return;
        }
    }
    old_pin = atoi(pin_str);

    if (hash_user_pin(old_pin) != active_acc->hashedPin) {
        printf("\n[ERROR] Incorrect current PIN.\n");
        return;
    }

    int new_pin = 0;
    printf(" Set new 4-Digit PIN: ");
    read_string(pin_str, sizeof(pin_str));

    if (strlen(pin_str) != 4) {
        printf("\n[ERROR] PIN must be exactly 4 digits.\n");
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)pin_str[i])) {
            printf("\n[ERROR] PIN must contain only digits.\n");
            return;
        }
    }
    new_pin = atoi(pin_str);

    active_acc->hashedPin = hash_user_pin(new_pin);
    show_loading_effect(" Updating security PIN");

    if (save_account(active_acc)) {
        printf("\n[SUCCESS] PIN changed successfully!\n");
        char details[256];
        snprintf(details, sizeof(details), "PIN changed for account: %06d", active_acc->accountNumber);
        log_audit("CHANGE_PIN", details);
    } else {
        printf("\n[ERROR] Failed to save updated PIN to disk.\n");
    }
}

void print_user_transaction_history(int account_num) {
    FILE *file = fopen(LOGS_FILE, "r");
    if (!file) {
        printf("\n[INFO] No transaction history exists on this server yet.\n");
        return;
    }

    char line[256];
    char block[1024] = "";
    char from_pattern[32];
    char to_pattern[32];
    snprintf(from_pattern, sizeof(from_pattern), "FROM: %06d", account_num);
    snprintf(to_pattern, sizeof(to_pattern), "TO: %06d", account_num);

    int in_block = 0;
    int match = 0;
    int count = 0;

    printf("\n========================================================================\n");
    printf("                  TRANSACTION HISTORY FOR ACCOUNT %06d\n", account_num);
    printf("========================================================================\n");

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') {
            if (in_block && match) {
                printf("%s", block);
                printf("------------------------------------------------------------------------\n");
                count++;
            }
            strcpy(block, line);
            in_block = 1;
            match = 0;
        } else {
            if (in_block) {
                // Buffer line into active block
                if (strlen(block) + strlen(line) < sizeof(block) - 1) {
                    strcat(block, line);
                }
                if (strstr(line, from_pattern) != NULL || strstr(line, to_pattern) != NULL) {
                    match = 1;
                }
            }
        }
    }

    if (in_block && match) {
        printf("%s", block);
        printf("------------------------------------------------------------------------\n");
        count++;
    }

    if (count == 0) {
        printf(" No transaction history records found for this account.\n");
    }
    printf("========================================================================\n");
    fclose(file);
}

void user_session_loop(Account *active_acc) {
    int choice = 0;
    while (1) {
        // Reload account from database in case of updates (e.g. transfers received)
        load_account(active_acc->accountNumber, active_acc);

        // Check if account was frozen by admin while logged in
        if (active_acc->isFrozen) {
            printf("\n[ALERT] This account has been frozen by an administrator. Session terminated.\n");
            press_enter_to_continue();
            break;
        }

        display_main_menu(active_acc->name, active_acc->accountNumber);

        if (!read_int(&choice)) {
            printf("\n[ERROR] Invalid option.\n");
            press_enter_to_continue();
            continue;
        }

        if (choice == 1) {
            handle_deposit(active_acc);
            press_enter_to_continue();
        } else if (choice == 2) {
            handle_withdraw(active_acc);
            press_enter_to_continue();
        } else if (choice == 3) {
            handle_transfer(active_acc);
            press_enter_to_continue();
        } else if (choice == 4) {
            print_header("ACCOUNT BALANCE");
            printf("\n------------------------------------------------------------\n");
            printf("  Account Holder  : %s\n", active_acc->name);
            printf("  Account Number  : %06d\n", active_acc->accountNumber);
            printf("  Current Balance : $%.2f\n", active_acc->balance);
            printf("------------------------------------------------------------\n");
            press_enter_to_continue();
        } else if (choice == 5) {
            print_header("TRANSACTION HISTORY");
            print_user_transaction_history(active_acc->accountNumber);
            press_enter_to_continue();
        } else if (choice == 6) {
            handle_change_pin(active_acc);
            press_enter_to_continue();
        } else if (choice == 7) {
            show_loading_effect(" Terminating session");
            printf("\n🔐 You have successfully logged out.\n");
            press_enter_to_continue();
            break;
        } else {
            printf("\n[ERROR] Selection out of range.\n");
            press_enter_to_continue();
        }
    }
}
