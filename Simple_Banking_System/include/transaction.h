#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "account.h"

/**
 * @brief Processes a deposit.
 * @param acc User account pointer.
 * @param amount Amount to deposit.
 * @return int 1 on success, 0 on failure.
 */
int deposit(Account *acc, double amount);

/**
 * @brief Processes a withdrawal.
 * @param acc User account pointer.
 * @param amount Amount to withdraw.
 * @return int 1 on success, 0 on failure.
 */
int withdraw(Account *acc, double amount);

/**
 * @brief Processes a fund transfer between two users.
 * @param sender Source user account pointer.
 * @param receiver Destination user account pointer.
 * @param amount Transfer amount.
 * @return int 1 on success, 0 on failure.
 */
int transfer(Account *sender, Account *receiver, double amount);

#endif /* TRANSACTION_H */
