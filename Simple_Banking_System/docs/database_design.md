# Database Design & Storage Schema — Simple Banking System

This document outlines the schema layout, record serialization, and file structures of the banking backend.

---

## 1. Persistent Records Layout

The database consists of two binary flat files (`data/accounts.dat`, `data/admins.dat`) and two plaintext log files (`data/transactions.log`, `data/audit.log`).

### User Account Model Schema (`data/accounts.dat`)
Saved as serialized binary blocks of the `Account` struct:
```c
typedef struct {
    int accountNumber;      // 6-digit unique identifier (e.g. 482716)
    char name[100];         // User Full Name string
    int hashedPin;          // Encrypted PIN integer
    double balance;         // Current account balance (double precision)
    int isFrozen;           // Boolean freeze flag (0 = Active, 1 = Frozen)
} Account;
```

### Admin Profile Schema (`data/admins.dat`)
Admin details are stored as:
```c
typedef struct {
    char username[50];      // Administrative username
    char password[50];      // Administrative password
} Admin;
```

---

## 2. File Update Strategies

### Append Action (Registration)
When a new user registration completes, the file pointer seeks to the end of `accounts.dat` using `fseek(file, 0, SEEK_END)` and appends the new struct block directly using `fwrite`.

### Inline Overwrite Action (Updates)
When updating account records (deposits, withdrawals, transfers, freezes), the system implements direct sector updates:
1.  Open `accounts.dat` in `r+b` mode.
2.  Iterate and read `sizeof(Account)` blocks.
3.  Compare the read account number with the target ID.
4.  Once matched, seek backward relative to the current position by exactly one struct size:
    `fseek(file, -(long)sizeof(Account), SEEK_CUR)`
5.  Overwrite the record with `fwrite` and close the stream.

### Safe Deletion Action
Because binary files cannot easily be truncated in the middle, deletions are executed via a temporary file:
1.  Open `accounts.dat` in `"rb"` and `"data/temp.dat"` in `"wb"`.
2.  Read all records sequentially.
3.  If the record matches the target ID, skip writing it.
4.  Otherwise, write it to `temp.dat`.
5.  Close both file descriptors.
6.  Call `remove("data/accounts.dat")` followed by `rename("data/temp.dat", "data/accounts.dat")`.
