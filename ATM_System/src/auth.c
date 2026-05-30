#include <stdio.h>
#include "auth.h"
#include "file_handler.h"

int hash_pin(int pin) {
    // Simple hashing formula simulation as required: pin * 7 + 13
    return (pin * 7) + 13;
}

int authenticate_user(int account_num, int pin, Account *authenticated_acc) {
    // Try to load user account by number
    if (!load_account(account_num, authenticated_acc)) {
        return -1; // Account does not exist
    }

    // Verify hashed PIN value match
    if (authenticated_acc->hashedPin == hash_pin(pin)) {
        return 1; // Authentication successful
    }

    return 0; // Incorrect PIN
}
