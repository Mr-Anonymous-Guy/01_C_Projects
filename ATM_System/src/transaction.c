#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaction.h"
#include "file_handler.h"
#include "utils.h"

int deposit_money(Account *acc, double amount) {
    if (amount <= 0.0) {
        return 0;
    }

    acc->balance += amount;
    if (!save_account(acc)) {
        // Rollback state in-memory if disk save failed
        acc->balance -= amount;
        return 0;
    }

    // Write to transaction ledger
    log_transaction(acc->accountNumber, "DEPOSIT", amount, acc->balance);
    return 1;
}

int withdraw_money(Account *acc, double amount) {
    if (amount <= 0.0) {
        return 0;
    }

    if (acc->balance < amount) {
        return 0; // Overdraft protection triggered
    }

    acc->balance -= amount;
    if (!save_account(acc)) {
        // Rollback state in-memory if disk save failed
        acc->balance += amount;
        return 0;
    }

    // Write to transaction ledger
    log_transaction(acc->accountNumber, "WITHDRAW", amount, acc->balance);
    return 1;
}

void print_transaction_history(int account_num) {
    FILE *file = fopen(LOGS_FILE, "r");
    if (!file) {
        printf("\n------------------------------------------------------------------------\n");
        printf(" [INFO] No transaction history exists on this machine yet.\n");
        printf("------------------------------------------------------------------------\n");
        return;
    }

    char line[512];
    char search_pattern[64];
    snprintf(search_pattern, sizeof(search_pattern), "Account: %06d", account_num);

    int count = 0;
    printf("\n========================================================================\n");
    printf("                  TRANSACTION HISTORY FOR ACCOUNT %06d\n", account_num);
    printf("========================================================================\n");

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, search_pattern) != NULL) {
            printf(" %s", line);
            count++;
        }
    }

    if (count == 0) {
        printf(" No transaction history records found for this account.\n");
    }
    printf("========================================================================\n");

    fclose(file);
}
