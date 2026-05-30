#ifndef LOGGER_H
#define LOGGER_H

/**
 * @brief Logs a transaction event to transactions.log.
 * @param type The type of transaction (DEPOSIT, WITHDRAW, TRANSFER).
 * @param from_acc Source account number (0 if N/A).
 * @param to_acc Destination account number (0 if N/A).
 * @param amount Transaction amount.
 * @param status Transaction status (SUCCESS or FAILED).
 * @return int 1 on success, 0 on failure.
 */
int log_transaction(const char *type, int from_acc, int to_acc, double amount, const char *status);

/**
 * @brief Logs an administrative/security audit event to audit.log.
 * @param action The action performed (e.g. LOGIN_FAILED, FREEZE_ACCOUNT).
 * @param details Detailed description.
 * @return int 1 on success, 0 on failure.
 */
int log_audit(const char *action, const char *details);

#endif /* LOGGER_H */
