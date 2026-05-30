#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_NAME_LEN 50

typedef struct {
    int accountNumber;
    char name[MAX_NAME_LEN];
    int hashedPin;
    double balance;
} Account;

/**
 * @brief Registers a new account with a name, PIN, and initial balance.
 * @param name User's name.
 * @param pin 4-digit PIN.
 * @param initial_balance Starting balance.
 * @param new_acc Pointer to Account struct where the details will be stored.
 * @return int 1 if successful, 0 if validation fails.
 */
int create_new_account(const char *name, int pin, double initial_balance, Account *new_acc);

/**
 * @brief Generates a unique 6-digit account number.
 * @return int The unique account number.
 */
int generate_unique_account_number(void);

/**
 * @brief Checks if a name is valid (only letters and spaces, not empty).
 * @param name The name to check.
 * @return int 1 if valid, 0 if invalid.
 */
int validate_name(const char *name);

#endif /* ACCOUNT_H */
