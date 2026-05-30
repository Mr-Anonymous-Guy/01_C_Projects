#ifndef AUTH_H
#define AUTH_H

#include "account.h"

#define MAX_LOGIN_RETRIES 3

/**
 * @brief Hashes a PIN using a secure-simulation algorithm.
 * @param pin The 4-digit PIN.
 * @return int The hashed PIN.
 */
int hash_pin(int pin);

/**
 * @brief Authenticates a user.
 * @param account_num The account number.
 * @param pin The 4-digit PIN.
 * @param authenticated_acc Pointer to Account to load details if authentication is successful.
 * @return int 1 if authenticated, 0 if incorrect PIN, -1 if account does not exist.
 */
int authenticate_user(int account_num, int pin, Account *authenticated_acc);

#endif /* AUTH_H */
