#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_NAME_LEN 100

typedef struct {
    int accountNumber;
    char name[MAX_NAME_LEN];
    int hashedPin;
    double balance;
    int isFrozen;
} Account;

/**
 * @brief Registers a new user account with validated details.
 * @param name User name.
 * @param pin 4-digit PIN.
 * @param initial_deposit Starting balance.
 * @param new_acc Pointer to store the created Account.
 * @return int 1 on success, 0 on validation error.
 */
int create_new_account(const char *name, int pin, double initial_deposit, Account *new_acc);

/**
 * @brief Verifies if an alphabetical name is valid (letters and spaces only, not empty).
 * @param name The name string.
 * @return int 1 if valid, 0 otherwise.
 */
int validate_name(const char *name);

/**
 * @brief Generates a collision-free unique 6-digit account number.
 * @return int The account number.
 */
int generate_unique_account_number(void);

#endif /* ACCOUNT_H */
