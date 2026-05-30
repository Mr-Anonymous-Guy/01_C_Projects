#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"
#include "file_handler.h"
#include "utils.h"
#include "logger.h"
#include "menu.h"

void view_all_accounts(void) {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        printf("\n[INFO] No accounts exist in the system yet.\n");
        return;
    }

    Account acc;
    printf("\n================================================================================\n");
    printf("                                ALL REGISTERED ACCOUNTS                          \n");
    printf("================================================================================\n");
    printf(" %-10s | %-30s | %-15s | %-10s\n", "Acc Number", "Account Holder Name", "Balance ($)", "Status");
    printf("--------------------------------------------------------------------------------\n");

    int count = 0;
    while (fread(&acc, sizeof(Account), 1, file) == 1) {
        printf(" %06d     | %-30s | $%-14.2f | %-10s\n",
               acc.accountNumber,
               acc.name,
               acc.balance,
               acc.isFrozen ? "[FROZEN]" : "[ACTIVE]");
        count++;
    }
    printf("--------------------------------------------------------------------------------\n");
    printf(" Total Accounts found: %d\n", count);
    printf("================================================================================\n");

    fclose(file);
}

void search_accounts_admin(void) {
    char search_str[100];
    printf("\nEnter name or substring to search: ");
    read_string(search_str, sizeof(search_str));

    if (strlen(search_str) == 0) {
        printf("\n[ERROR] Search query cannot be empty.\n");
        return;
    }

    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        printf("\n[INFO] No accounts database found.\n");
        return;
    }

    Account acc;
    int count = 0;
    printf("\n================================================================================\n");
    printf("                                SEARCH RESULTS                                  \n");
    printf("================================================================================\n");
    printf(" %-10s | %-30s | %-15s | %-10s\n", "Acc Number", "Account Holder Name", "Balance ($)", "Status");
    printf("--------------------------------------------------------------------------------\n");

    while (fread(&acc, sizeof(Account), 1, file) == 1) {
        char name_lower[MAX_NAME_LEN];
        char search_lower[100];

        // Convert structures to lower for case-insensitive matches
        int i;
        for (i = 0; acc.name[i]; i++) {
            name_lower[i] = tolower((unsigned char)acc.name[i]);
        }
        name_lower[i] = '\0';

        for (i = 0; search_str[i]; i++) {
            search_lower[i] = tolower((unsigned char)search_str[i]);
        }
        search_lower[i] = '\0';

        if (strstr(name_lower, search_lower) != NULL) {
            printf(" %06d     | %-30s | $%-14.2f | %-10s\n",
                   acc.accountNumber,
                   acc.name,
                   acc.balance,
                   acc.isFrozen ? "[FROZEN]" : "[ACTIVE]");
            count++;
        }
    }
    printf("--------------------------------------------------------------------------------\n");
    printf(" Search complete. %d records matched.\n", count);
    printf("================================================================================\n");

    fclose(file);
}

void toggle_freeze_account(void) {
    int acc_num;
    printf("\nEnter Account Number (6-digits) to Toggle Freeze: ");
    char input_str[16];
    read_string(input_str, sizeof(input_str));

    if (strlen(input_str) != 6) {
        printf("\n[ERROR] Account Number must be exactly 6 digits.\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)input_str[i])) {
            printf("\n[ERROR] Account Number must contain only digits.\n");
            return;
        }
    }
    acc_num = atoi(input_str);

    Account acc;
    if (!load_account(acc_num, &acc)) {
        printf("\n[ERROR] Account %06d does not exist.\n", acc_num);
        return;
    }

    acc.isFrozen = !acc.isFrozen;
    if (save_account(&acc)) {
        printf("\n[SUCCESS] Account %06d status updated successfully!\n", acc_num);
        printf(" New Status: %s\n", acc.isFrozen ? "[FROZEN]" : "[ACTIVE]");

        char details[256];
        snprintf(details, sizeof(details), "Account: %06d, New Status: %s",
                 acc.accountNumber, acc.isFrozen ? "FROZEN" : "UNFROZEN");
        log_audit("TOGGLE_FREEZE", details);
    } else {
        printf("\n[ERROR] Failed to save updated account state to database.\n");
    }
}

void delete_account_admin(void) {
    int acc_num;
    printf("\nEnter Account Number (6-digits) to DELETE: ");
    char input_str[16];
    read_string(input_str, sizeof(input_str));

    if (strlen(input_str) != 6) {
        printf("\n[ERROR] Account Number must be exactly 6 digits.\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)input_str[i])) {
            printf("\n[ERROR] Account Number must contain only digits.\n");
            return;
        }
    }
    acc_num = atoi(input_str);

    Account acc;
    if (!load_account(acc_num, &acc)) {
        printf("\n[ERROR] Account %06d does not exist.\n", acc_num);
        return;
    }

    printf(" WARNING: You are about to delete account %06d (%s) with balance $%.2f.\n",
           acc.accountNumber, acc.name, acc.balance);
    printf(" Are you sure you want to proceed? (Y/N): ");
    char confirm[10];
    read_string(confirm, sizeof(confirm));

    if (strcmp(confirm, "Y") != 0 && strcmp(confirm, "y") != 0) {
        printf("\n[INFO] Deletion cancelled.\n");
        return;
    }

    show_loading_effect(" Deleting account record");
    int res = delete_account(acc_num);
    if (res == 1) {
        printf("\n[SUCCESS] Account %06d deleted successfully.\n", acc_num);

        char details[256];
        snprintf(details, sizeof(details), "Deleted Account: %06d, Holder: %s, Residual Balance: $%.2f",
                 acc.accountNumber, acc.name, acc.balance);
        log_audit("DELETE_ACCOUNT", details);
    } else {
        printf("\n[ERROR] Failed to delete account from file system.\n");
    }
}

void view_audit_logs(void) {
    FILE *file = fopen(AUDIT_FILE, "r");
    if (!file) {
        printf("\n[INFO] Audit logs are empty.\n");
        return;
    }

    char line[512];
    printf("\n================================================================================\n");
    printf("                               BANK SECURITY AUDIT TRAIL                        \n");
    printf("================================================================================\n");
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        printf(" %s", line);
        count++;
    }
    if (count == 0) {
        printf(" No audit logs found.\n");
    }
    printf("================================================================================\n");
    fclose(file);
}

void view_bank_statistics(void) {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        printf("\n[INFO] No accounts exist yet. Bank statistics are blank.\n");
        return;
    }

    Account acc;
    double total_reserves = 0.0;
    double max_balance = -1.0;
    double min_balance = -1.0;
    int total_accounts = 0;
    int frozen_accounts = 0;

    int max_acc = 0;
    int min_acc = 0;

    while (fread(&acc, sizeof(Account), 1, file) == 1) {
        total_accounts++;
        total_reserves += acc.balance;
        if (acc.isFrozen) {
            frozen_accounts++;
        }

        if (max_balance < 0.0 || acc.balance > max_balance) {
            max_balance = acc.balance;
            max_acc = acc.accountNumber;
        }

        if (min_balance < 0.0 || acc.balance < min_balance) {
            min_balance = acc.balance;
            min_acc = acc.accountNumber;
        }
    }
    fclose(file);

    printf("\n================================================================================\n");
    printf("                               SYSTEM BANK METRICS                              \n");
    printf("================================================================================\n");
    printf("  Total Reserves (Bank Capital) : $%.2f\n", total_reserves);
    printf("  Total Registered Accounts     : %d\n", total_accounts);
    printf("  Active Accounts               : %d\n", total_accounts - frozen_accounts);
    printf("  Frozen Accounts               : %d\n", frozen_accounts);

    if (total_accounts > 0) {
        printf("  Highest Customer Balance      : $%.2f (Account: %06d)\n", max_balance, max_acc);
        printf("  Lowest Customer Balance       : $%.2f (Account: %06d)\n", min_balance, min_acc);
    } else {
        printf("  Highest Customer Balance      : N/A\n");
        printf("  Lowest Customer Balance       : N/A\n");
    }
    printf("================================================================================\n");
}

void admin_dashboard_menu(void) {
    int choice = 0;
    while (1) {
        print_header("ADMIN DASHBOARD CONTROL PANEL");
        printf("  [1] View All Customer Accounts\n");
        printf("  [2] Search Customer Account\n");
        printf("  [3] Freeze / Unfreeze Account\n");
        printf("  [4] Delete Customer Account\n");
        printf("  [5] View Security Audit Logs\n");
        printf("  [6] View Bank Balance & Statistics\n");
        printf("  [7] Logout Administrative Session\n");
        print_separator('=', 60);
        printf(" Selection (1-7): ");

        if (!read_int(&choice)) {
            printf("\n[ERROR] Invalid option.\n");
            press_enter_to_continue();
            continue;
        }

        if (choice == 1) {
            view_all_accounts();
            press_enter_to_continue();
        } else if (choice == 2) {
            search_accounts_admin();
            press_enter_to_continue();
        } else if (choice == 3) {
            toggle_freeze_account();
            press_enter_to_continue();
        } else if (choice == 4) {
            delete_account_admin();
            press_enter_to_continue();
        } else if (choice == 5) {
            view_audit_logs();
            press_enter_to_continue();
        } else if (choice == 6) {
            view_bank_statistics();
            press_enter_to_continue();
        } else if (choice == 7) {
            show_loading_effect(" Ending admin session");
            printf("\n[INFO] Administrative session logged out.\n");
            break;
        } else {
            printf("\n[ERROR] Selection out of range.\n");
            press_enter_to_continue();
        }
    }
}
