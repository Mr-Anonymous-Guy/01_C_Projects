#ifndef ADMIN_H
#define ADMIN_H

#define MAX_ADMIN_USERNAME 50
#define MAX_ADMIN_PASSWORD 50

typedef struct {
    char username[MAX_ADMIN_USERNAME];
    char password[MAX_ADMIN_PASSWORD];
} Admin;

/**
 * @brief Launches the admin dashboard menu loop.
 */
void admin_dashboard_menu(void);

/**
 * @brief Displays a formatted list of all registered accounts.
 */
void view_all_accounts(void);

/**
 * @brief Searches for user accounts by name (substring search).
 */
void search_accounts_admin(void);

/**
 * @brief Freezes or unfreezes a target user account.
 */
void toggle_freeze_account(void);

/**
 * @brief Removes a user account entirely from the database.
 */
void delete_account_admin(void);

/**
 * @brief Displays the audit logs trail.
 */
void view_audit_logs(void);

/**
 * @brief Displays system-wide banking statistics (assets, reserves).
 */
void view_bank_statistics(void);

#endif /* ADMIN_H */
