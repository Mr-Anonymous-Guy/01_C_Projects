#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "account.h"
#include "admin.h"

#define ACCOUNTS_FILE "data/accounts.dat"
#define ADMINS_FILE "data/admins.dat"
#define LOGS_FILE "data/transactions.log"
#define AUDIT_FILE "data/audit.log"

/**
 * @brief Ensures data/ directory exists and prepopulates default admin credentials if missing.
 */
void ensure_data_directory(void);

/**
 * @brief Saves a new account or updates an existing account in accounts.dat.
 * @param acc The account to save.
 * @return int 1 on success, 0 on failure.
 */
int save_account(const Account *acc);

/**
 * @brief Loads account details by account number.
 * @param account_num The account number.
 * @param acc Destination pointer.
 * @return int 1 on success (found), 0 on failure (not found), -1 on error.
 */
int load_account(int account_num, Account *acc);

/**
 * @brief Checks if an account exists.
 * @param account_num The account number.
 * @return int 1 if exists, 0 otherwise.
 */
int account_exists(int account_num);

/**
 * @brief Deletes an account from accounts.dat.
 * @param account_num The account number to delete.
 * @return int 1 on success, 0 if not found, -1 on error.
 */
int delete_account(int account_num);

/**
 * @brief Loads a single admin profile by username.
 * @param username The username.
 * @param adm Output destination.
 * @return int 1 if found, 0 otherwise.
 */
int load_admin(const char *username, Admin *adm);

/**
 * @brief Appends to plain-text logs.
 * @param file_path The path to the log file (transactions.log or audit.log).
 * @param log_entry The text entry to write.
 * @return int 1 on success, 0 on failure.
 */
int append_log(const char *file_path, const char *log_entry);

#endif /* FILE_HANDLER_H */
