#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "account.h"
#include "auth.h"
#include "transaction.h"
#include "file_handler.h"
#include "logger.h"
#include "menu.h"
#include "utils.h"
#include "bank.h"
#include "admin.h"

// Helper function to read exactly a 4-digit PIN string and convert it
static int get_secure_pin(int *pin) {
    char pin_str[16];
    read_string(pin_str, sizeof(pin_str));
    if (strlen(pin_str) != 4) {
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)pin_str[i])) {
            return 0;
        }
    }
    *pin = atoi(pin_str);
    return 1;
}

// Helper function to read exactly a 6-digit account number string and convert it
static int get_account_number_input(int *acc_num) {
    char acc_str[16];
    read_string(acc_str, sizeof(acc_str));
    if (strlen(acc_str) != 6) {
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)acc_str[i])) {
            return 0;
        }
    }
    *acc_num = atoi(acc_str);
    return 1;
}

int main(void) {
    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Prepare system database path files
    ensure_data_directory();

    int choice = 0;
    while (1) {
        display_welcome_menu();
        if (!read_int(&choice)) {
            printf("\n[ERROR] Invalid menu choice. Please select 1, 2, 3, or 4.\n");
            press_enter_to_continue();
            continue;
        }

        if (choice == 1) { // Create Account
            print_header("CREATE NEW ACCOUNT");
            char name[100];
            int pin = 0;
            double initial_deposit = 0.0;

            printf(" Enter Full Name: ");
            read_string(name, sizeof(name));

            if (!validate_name(name)) {
                printf("\n[ERROR] Invalid Name format. Use letters and spaces only.\n");
                press_enter_to_continue();
                continue;
            }

            printf(" Set a 4-Digit PIN: ");
            if (!get_secure_pin(&pin)) {
                printf("\n[ERROR] Invalid PIN. PIN must be exactly 4 digits (e.g., 1234).\n");
                press_enter_to_continue();
                continue;
            }

            printf(" Initial Deposit Amount ($): ");
            if (!read_double(&initial_deposit) || initial_deposit < 0.0) {
                printf("\n[ERROR] Invalid amount. Deposit must be a positive number.\n");
                press_enter_to_continue();
                continue;
            }

            Account new_acc;
            show_loading_effect(" Opening account in core ledger");
            if (create_new_account(name, pin, initial_deposit, &new_acc)) {
                printf("\n🚀 Account successfully registered!\n");
                print_separator('-', 40);
                printf(" Account Number : %06d\n", new_acc.accountNumber);
                printf(" Holder Name    : %s\n", new_acc.name);
                printf(" Account Balance: $%.2f\n", new_acc.balance);
                print_separator('-', 40);
                printf(" Please write down your Account Number. You will need it to login.\n");
            } else {
                printf("\n[ERROR] Failed to save account record to database.\n");
            }
            press_enter_to_continue();

        } else if (choice == 2) { // User Login
            print_header("SECURE USER LOGIN");
            int account_num = 0;

            printf(" Enter Account Number (6-Digits): ");
            if (!get_account_number_input(&account_num)) {
                printf("\n[ERROR] Account Number must be exactly 6 digits.\n");
                press_enter_to_continue();
                continue;
            }

            Account active_acc;
            if (!load_account(account_num, &active_acc)) {
                printf("\n[ERROR] Account %06d does not exist.\n", account_num);
                press_enter_to_continue();
                continue;
            }

            if (active_acc.isFrozen) {
                printf("\n[ALERT] This account is frozen. Please contact a bank administrator.\n");
                press_enter_to_continue();
                continue;
            }

            int authenticated = 0;
            int retries = 0;
            int pin = 0;

            while (retries < MAX_LOGIN_RETRIES) {
                printf(" Enter 4-Digit PIN (Attempt %d/%d): ", retries + 1, MAX_LOGIN_RETRIES);
                if (!get_secure_pin(&pin)) {
                    printf("[ERROR] PIN must be exactly 4 digits.\n");
                    retries++;
                    continue;
                }

                int auth_res = authenticate_user(account_num, pin, &active_acc);
                if (auth_res == 1) {
                    authenticated = 1;
                    break;
                } else if (auth_res == -1) {
                    printf("[ERROR] Account is frozen.\n");
                    break;
                } else {
                    printf("[ERROR] Incorrect PIN value.\n");
                    retries++;
                }
            }

            if (!authenticated) {
                if (retries >= MAX_LOGIN_RETRIES) {
                    // Update disk database state to frozen status
                    active_acc.isFrozen = 1;
                    save_account(&active_acc);
                    printf("\n[ALERT] Too many incorrect PIN attempts. Your account has been locked.\n");
                    printf(" Please contact a bank administrator to restore access.\n");

                    char details[256];
                    snprintf(details, sizeof(details), "Account %06d frozen due to too many incorrect login attempts", account_num);
                    log_audit("ACCOUNT_LOCKOUT", details);
                } else {
                    printf("\n[ERROR] Login failed.\n");
                }
                press_enter_to_continue();
                continue;
            }

            show_loading_effect(" Accessing bank dashboard");
            user_session_loop(&active_acc);

        } else if (choice == 3) { // Admin Login
            print_header("SECURE ADMINISTRATIVE ACCESS");
            char username[100];
            char password[100];

            printf(" Enter Admin Username: ");
            read_string(username, sizeof(username));
            printf(" Enter Admin Password: ");
            read_string(password, sizeof(password));

            show_loading_effect(" Authenticating admin session");
            if (authenticate_admin(username, password)) {
                char details[256];
                snprintf(details, sizeof(details), "Admin user '%s' logged in successfully", username);
                log_audit("ADMIN_LOGIN", details);
                
                admin_dashboard_menu();
            } else {
                printf("\n[ERROR] Access Denied. Invalid admin credentials.\n");
                char details[256];
                snprintf(details, sizeof(details), "Failed admin login attempt with username: '%s'", username);
                log_audit("ADMIN_LOGIN_FAIL", details);
                press_enter_to_continue();
            }

        } else if (choice == 4) { // Exit
            print_header("");
            printf("\n Thank you for using the Simple Banking System. Goodbye!\n\n");
            break;
        } else {
            printf("\n[ERROR] Selection out of range.\n");
            press_enter_to_continue();
        }
    }

    return 0;
}
