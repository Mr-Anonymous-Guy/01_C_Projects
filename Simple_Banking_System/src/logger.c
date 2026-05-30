#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "file_handler.h"
#include "utils.h"

int log_transaction(const char *type, int from_acc, int to_acc, double amount, const char *status) {
    char timestamp[30];
    get_timestamp(timestamp, sizeof(timestamp));

    char from_str[32];
    char to_str[32];

    if (from_acc == 0) {
        strcpy(from_str, "N/A");
    } else {
        snprintf(from_str, sizeof(from_str), "%06d", from_acc);
    }

    if (to_acc == 0) {
        strcpy(to_str, "N/A");
    } else {
        snprintf(to_str, sizeof(to_str), "%06d", to_acc);
    }

    char log_entry[512];
    snprintf(log_entry, sizeof(log_entry),
             "[%s]\n%s\nFROM: %s\nTO: %s\nAMOUNT: %.2f\nSTATUS: %s\n\n",
             timestamp, type, from_str, to_str, amount, status);

    return append_log(LOGS_FILE, log_entry);
}

int log_audit(const char *action, const char *details) {
    char timestamp[30];
    get_timestamp(timestamp, sizeof(timestamp));

    char log_entry[512];
    snprintf(log_entry, sizeof(log_entry),
             "[%s] ACTION: %-15s | DETAILS: %s\n",
             timestamp, action, details);

    return append_log(AUDIT_FILE, log_entry);
}
