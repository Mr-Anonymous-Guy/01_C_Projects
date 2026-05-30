# System Scalability & Future Upgrade Path — Simple Banking System

This document outlines the upgrade path and scaling strategies for transitioning the C flat-file banking core to a production-grade enterprise system.

---

## 1. Database Layer Upgrades (Flat Files to SQL)

Currently, the system relies on binary records in `accounts.dat`. To scale for concurrent operations, backups, and relational integrity, the persistence layer can be refactored:

### Integration with SQLite/MySQL
1.  **Refactoring the persistence interface**:
    The system's UI and banking engine communicate via `file_handler.h` APIs (e.g. `load_account(int num, Account *acc)`).
2.  **Schema Migration**:
    Create an SQL schema matching the existing data structures:
    ```sql
    CREATE TABLE accounts (
        account_number INT PRIMARY KEY,
        name VARCHAR(100) NOT NULL,
        hashed_pin INT NOT NULL,
        balance DOUBLE PRECISION DEFAULT 0.0,
        is_frozen INT DEFAULT 0
    );
    ```
3.  **Refactoring `file_handler.c`**:
    Replace standard library file reads (`fread`, `fwrite`) with library queries (e.g. `sqlite3_prepare_v2` or `mysql_query`). The signatures in `file_handler.h` will remain unchanged, preserving modularity.

---

## 2. Transitioning to Client-Server Architecture (Networking)

To support multiple remote ATMs or teller consoles connecting to a single central banking core, the project can be partitioned over TCP/IP sockets:

```text
  [ Client CLI Console ]  ---( TCP Sockets )--->  [ Central Banking Core Server ]
 (Translates UI menu inputs                     (Maintains account files, logs transactions,
   into payload packets)                         handles session validations and auth checks)
```

### Protocol Design
*   Define structured request/response packets:
    ```c
    typedef struct {
        int requestType; // 1 = Login, 2 = Deposit, 3 = Transfer
        int accNum;
        int targetAccNum;
        double amount;
        char payload[128];
    } BankPacket;
    ```
*   Use standard POSIX sockets (`sys/socket.h` on Linux/macOS) or Winsock (`winsock2.h` on Windows) to transmit packets.
*   Implement thread pools (`pthread` or standard worker queues) on the server to handle concurrent incoming client connections safely without race conditions on account updates.
