#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "file_handler.h"

int hash_user_pin(int pin) {
    // Simulated hashing formula: (pin * 11) + 17
    return (pin * 11) + 17;
}

int authenticate_user(int account_num, int pin, Account *acc) {
    int load_res = load_account(account_num, acc);
    if (!load_res) {
        return -2; // Account not found
    }

    if (acc->isFrozen) {
        return -1; // Block authentication for frozen accounts
    }

    if (acc->hashedPin == hash_user_pin(pin)) {
        return 1; // PIN matched
    }

    return 0; // PIN mismatched
}

int authenticate_admin(const char *username, const char *password) {
    Admin adm;
    if (!load_admin(username, &adm)) {
        return 0;
    }
    if (strcmp(adm.password, password) == 0) {
        return 1; // Username and password match
    }
    return 0;
}
