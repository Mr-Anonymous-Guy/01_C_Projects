#ifndef AUTH_H
#define AUTH_H

#include "account.h"

#define MAX_LOGIN_RETRIES 3

/**
 * @brief Hashes a user PIN using the banking core simulation formula: (pin * 11) + 17.
 * @param pin The raw 4-digit PIN.
 * @return int The hashed PIN.
 */
int hash_user_pin(int pin);

/**
 * @brief Verifies user credentials.
 * @param account_num The account number.
 * @param pin The 4-digit PIN.
 * @param acc Destination pointer to load the authenticated account data.
 * @return int 1 on success, 0 on incorrect PIN, -1 if frozen, -2 if not found.
 */
int authenticate_user(int account_num, int pin, Account *acc);

/**
 * @brief Authenticates an admin.
 * @param username Username.
 * @param password Password.
 * @return int 1 on success, 0 on incorrect credentials.
 */
int authenticate_admin(const char *username, const char *password);

#endif /* AUTH_H */
