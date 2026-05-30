#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "account.h"

/**
 * @brief Performs a deposit of a specified amount into the account.
 * @param acc Pointer to the active Account.
 * @param amount The amount to deposit.
 * @return int 1 if successful, 0 if validation fails.
 */
int deposit_money(Account *acc, double amount);

/**
 * @brief Performs a withdrawal of a specified amount from the account.
 * @param acc Pointer to the active Account.
 * @param amount The amount to withdraw.
 * @return int 1 if successful, 0 if insufficient funds or invalid amount.
 */
int withdraw_money(Account *acc, double amount);

/**
 * @brief Prints the transaction history for a specific account.
 * @param account_num The account number.
 */
void print_transaction_history(int account_num);

#endif /* TRANSACTION_H */
