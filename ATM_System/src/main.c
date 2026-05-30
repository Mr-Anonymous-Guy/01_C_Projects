#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "account.h"
#include "auth.h"
#include "transaction.h"
#include "file_handler.h"
#include "menu.h"
#include "utils.h"

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
    // Seed the random number generator once at start of program
    srand((unsigned int)time(NULL));

    // Ensure storage path setup
    ensure_data_directory();

    int choice = 0;
    while (1) {
        display_welcome_menu();
        if (!read_int(&choice)) {
            printf("\n[ERROR] Invalid menu choice. Please select 1, 2, or 3.\n");
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
            if (!read_float(&initial_deposit) || initial_deposit < 0.0) {
                printf("\n[ERROR] Invalid amount. Deposit must be a positive number.\n");
                press_enter_to_continue();
                continue;
            }

            Account new_acc;
            show_loading_effect(" Creating account");
            if (create_new_account(name, pin, initial_deposit, &new_acc)) {
                printf("\n🚀 Account successfully created!\n");
                print_separator('-', 40);
                printf(" Account Number : %06d\n", new_acc.accountNumber);
                printf(" Holder Name    : %s\n", new_acc.name);
                printf(" Opening Balance: $%.2f\n", new_acc.balance);
                print_separator('-', 40);
                printf(" Please write down your Account Number. You will need it to login.\n");
            } else {
                printf("\n[ERROR] Failed to create account. Please check inputs or write permissions.\n");
            }
            press_enter_to_continue();

        } else if (choice == 2) { // Login
            print_header("SECURE ACCOUNT LOGIN");
            int account_num = 0;
            int pin = 0;

            printf(" Enter Account Number (6-Digits): ");
            if (!get_account_number_input(&account_num)) {
                printf("\n[ERROR] Account Number must be exactly 6 digits.\n");
                press_enter_to_continue();
                continue;
            }

            // Check if account exists first to fail early
            if (!account_exists(account_num)) {
                printf("\n[ERROR] Account %06d does not exist.\n", account_num);
                press_enter_to_continue();
                continue;
            }

            int authenticated = 0;
            int retries = 0;
            Account active_acc;

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
                } else {
                    printf("[ERROR] Incorrect PIN value.\n");
                    retries++;
                }
            }

            if (!authenticated) {
                printf("\n[ALERT] Too many incorrect login attempts. Access denied.\n");
                press_enter_to_continue();
                continue;
            }

            show_loading_effect(" Accessing dashboard");

            // Session Dashboard Loop
            int user_choice = 0;
            int logout = 0;
            while (!logout) {
                // Ensure session state matches disk in case of external checks
                load_account(active_acc.accountNumber, &active_acc);

                display_main_menu(active_acc.name, active_acc.accountNumber);
                if (!read_int(&user_choice)) {
                    printf("\n[ERROR] Invalid option selected.\n");
                    press_enter_to_continue();
                    continue;
                }

                if (user_choice == 1) { // Deposit
                    print_header("DEPOSIT FUNDS");
                    float deposit_amt = 0.0f;
                    printf(" Enter deposit amount ($): ");
                    if (!read_float(&deposit_amt) || deposit_amt <= 0.0) {
                        printf("\n[ERROR] Deposit amount must be a positive number.\n");
                    } else {
                        show_loading_effect(" Processing deposit");
                        if (deposit_money(&active_acc, deposit_amt)) {
                            printf("\n✅ Success! Deposited $%.2f. New Balance: $%.2f\n", deposit_amt, active_acc.balance);
                        } else {
                            printf("\n[ERROR] Transaction failed. Could not write to disk.\n");
                        }
                    }
                    press_enter_to_continue();

                } else if (user_choice == 2) { // Withdraw
                    print_header("WITHDRAW FUNDS");
                    float withdraw_amt = 0.0f;
                    printf(" Current Available Balance: $%.2f\n", active_acc.balance);
                    printf(" Enter withdrawal amount ($): ");
                    if (!read_float(&withdraw_amt) || withdraw_amt <= 0.0) {
                        printf("\n[ERROR] Withdrawal amount must be a positive number.\n");
                    } else {
                        show_loading_effect(" Processing withdrawal");
                        if (withdraw_money(&active_acc, withdraw_amt)) {
                            printf("\n✅ Success! Withdrew $%.2f. Remaining Balance: $%.2f\n", withdraw_amt, active_acc.balance);
                        } else {
                            if (active_acc.balance < withdraw_amt) {
                                printf("\n[ERROR] Insufficient funds. Overdraft denied.\n");
                            } else {
                                printf("\n[ERROR] Transaction failed due to disk write error.\n");
                            }
                        }
                    }
                    press_enter_to_continue();

                } else if (user_choice == 3) { // Check Balance
                    print_header("ACCOUNT BALANCE");
                    printf("\n------------------------------------------------------------\n");
                    printf("  Account Holder  : %s\n", active_acc.name);
                    printf("  Account Number  : %06d\n", active_acc.accountNumber);
                    printf("  Current Balance : $%.2f\n", active_acc.balance);
                    printf("------------------------------------------------------------\n");
                    press_enter_to_continue();

                } else if (user_choice == 4) { // View Ledger
                    print_header("TRANSACTION HISTORY");
                    print_transaction_history(active_acc.accountNumber);
                    press_enter_to_continue();

                } else if (user_choice == 5) { // Logout
                    show_loading_effect(" Terminating session");
                    printf("\n🔐 You have successfully logged out.\n");
                    logout = 1;
                    press_enter_to_continue();

                } else {
                    printf("\n[ERROR] Invalid menu choice.\n");
                    press_enter_to_continue();
                }
            }

        } else if (choice == 3) { // Exit
            print_header("");
            printf("\n Thank you for using the ATM Management System. Goodbye!\n\n");
            break;
        } else {
            printf("\n[ERROR] Selection out of range.\n");
            press_enter_to_continue();
        }
    }

    return 0;
}
