#ifndef BANK_H
#define BANK_H

#include "account.h"

/**
 * @brief Coordinates workflows for a logged-in user session.
 * @param active_acc The active Account.
 */
void user_session_loop(Account *active_acc);

/**
 * @brief Helper function to parse transactions.log and display user specific transactions.
 * @param account_num The account number of the user.
 */
void print_user_transaction_history(int account_num);

#endif /* BANK_H */
