#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "account.h"
#include "file_handler.h"
#include "auth.h"
#include "logger.h"

int validate_name(const char *name) {
    int len = strlen(name);
    if (len == 0 || len >= MAX_NAME_LEN) {
        return 0;
    }

    int non_space = 0;
    for (int i = 0; i < len; i++) {
        if (!isalpha((unsigned char)name[i]) && name[i] != ' ') {
            return 0; // Contains digits or symbols
        }
        if (name[i] != ' ') {
            non_space = 1;
        }
    }
    return non_space; // Must not be empty or purely spaces
}

int generate_unique_account_number(void) {
    int num;
    // Note: srand() is called once in main.c to seed the generator
    do {
        // Generate a 6-digit random number between 100000 and 999999
        num = (rand() % 900000) + 100000;
    } while (account_exists(num));
    return num;
}

int create_new_account(const char *name, int pin, double initial_deposit, Account *new_acc) {
    if (!validate_name(name)) {
        return 0;
    }
    if (pin < 0 || pin > 9999) {
        return 0;
    }
    if (initial_deposit < 0.0) {
        return 0;
    }

    new_acc->accountNumber = generate_unique_account_number();
    strncpy(new_acc->name, name, MAX_NAME_LEN - 1);
    new_acc->name[MAX_NAME_LEN - 1] = '\0';
    new_acc->hashedPin = hash_user_pin(pin);
    new_acc->balance = initial_deposit;
    new_acc->isFrozen = 0;

    if (!save_account(new_acc)) {
        return 0;
    }

    // Write audit trail and transaction log entries
    log_transaction("CREATE", 0, new_acc->accountNumber, initial_deposit, "SUCCESS");

    char audit_details[128];
    snprintf(audit_details, sizeof(audit_details), "Account Number: %06d, Holder: %s, Initial Deposit: $%.2f",
             new_acc->accountNumber, name, initial_deposit);
    log_audit("ACCOUNT_CREATE", audit_details);

    return 1;
}
