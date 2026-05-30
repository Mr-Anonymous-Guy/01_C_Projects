#include <stdio.h>
#include "transaction.h"
#include "file_handler.h"
#include "logger.h"

int deposit(Account *acc, double amount) {
    if (amount <= 0.0) {
        log_transaction("DEPOSIT", 0, acc->accountNumber, amount, "FAILED (INVALID_AMOUNT)");
        return 0;
    }
    if (acc->isFrozen) {
        log_transaction("DEPOSIT", 0, acc->accountNumber, amount, "FAILED (FROZEN)");
        return 0;
    }

    acc->balance += amount;
    if (!save_account(acc)) {
        // Rollback changes
        acc->balance -= amount;
        log_transaction("DEPOSIT", 0, acc->accountNumber, amount, "FAILED (DISK_ERR)");
        return 0;
    }

    log_transaction("DEPOSIT", 0, acc->accountNumber, amount, "SUCCESS");
    return 1;
}

int withdraw(Account *acc, double amount) {
    if (amount <= 0.0) {
        log_transaction("WITHDRAW", acc->accountNumber, 0, amount, "FAILED (INVALID_AMOUNT)");
        return 0;
    }
    if (acc->isFrozen) {
        log_transaction("WITHDRAW", acc->accountNumber, 0, amount, "FAILED (FROZEN)");
        return 0;
    }
    if (acc->balance < amount) {
        log_transaction("WITHDRAW", acc->accountNumber, 0, amount, "FAILED (OVERDRAFT)");
        return 0;
    }

    acc->balance -= amount;
    if (!save_account(acc)) {
        // Rollback changes
        acc->balance += amount;
        log_transaction("WITHDRAW", acc->accountNumber, 0, amount, "FAILED (DISK_ERR)");
        return 0;
    }

    log_transaction("WITHDRAW", acc->accountNumber, 0, amount, "SUCCESS");
    return 1;
}

int transfer(Account *sender, Account *receiver, double amount) {
    if (amount <= 0.0) {
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (INVALID_AMOUNT)");
        return 0;
    }
    if (sender->isFrozen) {
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (SENDER_FROZEN)");
        return 0;
    }
    if (receiver->isFrozen) {
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (RECEIVER_FROZEN)");
        return 0;
    }
    if (sender->balance < amount) {
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (OVERDRAFT)");
        return 0;
    }
    if (sender->accountNumber == receiver->accountNumber) {
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (SAME_ACCOUNT)");
        return 0;
    }

    // Process double-entry transactional update in-memory
    sender->balance -= amount;
    receiver->balance += amount;

    // Save sender state first
    if (!save_account(sender)) {
        // Revert in-memory values
        sender->balance += amount;
        receiver->balance -= amount;
        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (DISK_ERR_SENDER)");
        return 0;
    }

    // Save receiver state next
    if (!save_account(receiver)) {
        // Revert receiver changes in-memory
        receiver->balance -= amount;
        // Revert sender balance on disk and in-memory
        sender->balance += amount;
        save_account(sender);

        log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "FAILED (DISK_ERR_RECEIVER)");
        return 0;
    }

    // Transaction successfully committed on both nodes
    log_transaction("TRANSFER", sender->accountNumber, receiver->accountNumber, amount, "SUCCESS");
    return 1;
}
