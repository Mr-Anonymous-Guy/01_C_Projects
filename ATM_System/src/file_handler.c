#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "utils.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

void ensure_data_directory(void) {
    MKDIR("data");
}

int save_account(const Account *acc) {
    ensure_data_directory();
    FILE *file = fopen(ACCOUNTS_FILE, "r+b");
    if (!file) {
        // If file does not exist, open in write-binary to create it
        file = fopen(ACCOUNTS_FILE, "wb");
        if (!file) {
            return 0;
        }
    }

    Account temp;
    int found = 0;

    // Scan for existing record to update
    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == acc->accountNumber) {
            // Seek back by one Account size to overwrite this record
            if (fseek(file, -(long)sizeof(Account), SEEK_CUR) != 0) {
                fclose(file);
                return 0;
            }
            if (fwrite(acc, sizeof(Account), 1, file) != 1) {
                fclose(file);
                return 0;
            }
            found = 1;
            break;
        }
    }

    // If not found, append to the end
    if (!found) {
        if (fseek(file, 0, SEEK_END) != 0) {
            fclose(file);
            return 0;
        }
        if (fwrite(acc, sizeof(Account), 1, file) != 1) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int load_account(int account_num, Account *acc) {
    ensure_data_directory();
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        return 0; // File does not exist yet, treat as not found
    }

    Account temp;
    int found = 0;
    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == account_num) {
            *acc = temp;
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

int account_exists(int account_num) {
    Account temp;
    return load_account(account_num, &temp);
}

int log_transaction(int account_num, const char *type, double amount, double new_balance) {
    ensure_data_directory();
    FILE *file = fopen(LOGS_FILE, "a");
    if (!file) {
        return 0;
    }

    char timestamp[30];
    get_timestamp(timestamp, sizeof(timestamp));

    // Formatted append for the transaction ledger
    fprintf(file, "[%s] Account: %06d | Type: %-8s | Amount: $%-10.2f | Balance: $%-10.2f\n",
            timestamp, account_num, type, amount, new_balance);

    fclose(file);
    return 1;
}
