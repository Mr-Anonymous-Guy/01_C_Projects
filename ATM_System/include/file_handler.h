#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "account.h"

#define ACCOUNTS_FILE "data/accounts.dat"
#define LOGS_FILE "data/transactions.log"

/**
 * @brief Ensures that the data directory and any metadata files exist.
 */
void ensure_data_directory(void);

/**
 * @brief Saves an account to the accounts file. If it already exists, updates it.
 * @param acc Pointer to the Account to save.
 * @return int 1 if successful, 0 otherwise.
 */
int save_account(const Account *acc);

/**
 * @brief Loads an account's details from the accounts file by account number.
 * @param account_num The account number to look up.
 * @param acc Pointer to store the loaded account details.
 * @return int 1 if successful, 0 if account not found, -1 if file error.
 */
int load_account(int account_num, Account *acc);

/**
 * @brief Logs a transaction to transactions.log file.
 * @param account_num The account number of the transaction owner.
 * @param type The type of transaction (e.g. "DEPOSIT", "WITHDRAW").
 * @param amount The transaction amount.
 * @param new_balance The resulting balance.
 * @return int 1 if successful, 0 otherwise.
 */
int log_transaction(int account_num, const char *type, float amount, float new_balance);

/**
 * @brief Checks if an account exists in the accounts file.
 * @param account_num The account number to check.
 * @return int 1 if it exists, 0 otherwise.
 */
int account_exists(int account_num);

#endif /* FILE_HANDLER_H */
