# 🏦 Simple Banking System Core in C

A production-grade, terminal-based banking backend core simulation written in standard C (C99). This project demonstrates a layered modular architecture, double-entry transactional consistency with error rollback controls, binary flat-file database persistence, administrative overrides, and system audit logging.

Designed as an internship-level portfolio systems programming project to showcase software engineering best practices.

---

## 📂 Project Directory Structure

```text
Simple_Banking_System/
├── src/                  # Code implementation modules (.c files)
│   ├── main.c            # Application entry point & welcome loops
│   ├── auth.c            # User/admin authentication & retry locking
│   ├── bank.c            # Dashboard workflows orchestrations
│   ├── account.c         # Account registrations & name validation
│   ├── transaction.c     # Credit/debit & money transfers logic
│   ├── admin.c           # Administrative override dashboards
│   ├── logger.c          # Append-only transaction & audit logging
│   ├── file_handler.c    # Low-level persistent binary stream queries
│   ├── menu.c            # Delimiters and CLI screen panels
│   └── utils.c           # Input validations & platform sleep/clears
├── include/              # Header declarations (.h files)
│   ├── auth.h
│   ├── bank.h
│   ├── account.h
│   ├── transaction.h
│   ├── admin.h
│   ├── logger.h
│   ├── file_handler.h
│   ├── menu.h
│   └── utils.h
├── data/                 # Local persistent files (ignored in Git)
│   ├── accounts.dat      # Serialized binary database of user accounts
│   ├── admins.dat        # Serialized binary database of admin users
│   ├── transactions.log  # Plaintext transaction logging
│   └── audit.log          # Security audit trails
├── build/                # Output binary folder
├── docs/                 # Architectural specifications
│   ├── architecture.md   # Layering & coupling rules
│   ├── database_design.md# Serialized struct schema offsets
│   ├── security.md       # PIN hashing & lockout algorithms
│   ├── banking_flow.md   # System flowcharts
│   └── scalability.md    # SQL migrations & client-server TCP networking
├── tests/                # Testing scenarios
│   ├── test_cases.txt    # Standard operations checks
│   ├── edge_cases.txt    # Boundaries & overflow constraints
│   └── functional_testing.md # QA matrix check sheets
├── Makefile              # GCC build script
├── run.sh                # Linux/macOS execution script
├── run.bat               # Windows build & run launcher script
├── .gitignore            # Git exclusion rules
├── LICENSE               # MIT License
└── README.md             # This documentation
```

---

## 🛡️ Security Features & Protections

1.  **PIN Hashing Simulation**: Plaintext PIN values are encrypted using the mathematical formula:
    $$\text{HashedPIN} = (\text{PlainPIN} \times 11) + 17$$
2.  **Brute Force Lockout**: Users are blocked after 3 consecutive wrong PIN entries. The core marks the record as `isFrozen = 1` on disk, preventing further sessions or transfers until unfrozen by an admin.
3.  **Credential Separation**: Administrative sessions authenticate against a separate database (`admins.dat`) using username/password records, separate from standard account numbers.
4.  **Audit Logs**: Every login failure, creation, deletion, or freeze action writes audit trails with timestamps to `data/audit.log`.

---

## 💸 Double-Entry Transaction Consistency

Transfers between customer balances maintain complete transactional safety. If writing the receiver's updated record fails on disk, the system rolls back the sender's balance in memory and updates the disk to prevent money duplication or disappearance.

```text
  [Sender Account] --(-$200)--> (Validation OK) --(Disk Write OK?) 
                                                         │
               ┌───────────────── YES ───────────────────┘
               ▼
  [Receiver Account] --(+$200)--> (Disk Write OK?) 
               │
               ├─ YES ──► Commit (SUCCESS)
               └─ NO  ──► Revert Sender balance disk sector ──► Rollback (FAILED)
```

---

## 🛠️ Build and Installation Instructions

### Prerequisites
*   GCC Compiler
*   GNU Make (for Linux/macOS)
*   MinGW-w64 (for Windows CMD/PowerShell)

### Linux / macOS / WSL
Ensure compile systems are installed, then run the helper script:
```bash
chmod +x run.sh
./run.sh
```
Or compile manually with `Makefile`:
*   `make` — compiles the source files to `build/bank_system`.
*   `make run` — compiles and launches the application.
*   `make debug` — compiles with `-g` debug flags.
*   `make clean` — deletes object files and binaries.
*   `make clean_all` — wipes build targets and user account files.

### Windows (CMD or PowerShell)
Double-click [run.bat](file:///c:/Mr-Anonymous-Guy/Mr-Anonymous-Guy/01_C_Projects/Simple_Banking_System/run.bat) or execute it in your terminal:
```cmd
run.bat
```
Supports flags:
*   `run.bat debug` — compiles in debug mode.
*   `run.bat release` — compiles with optimization flags (`-O2`).
*   `run.bat clean` — deletes the `build/` folder.

---

## 🖥️ User & Admin Interfaces

The terminal layout renders clean panels for both dashboards:

### Main welcome screen
```text
============================================================
                  SIMPLE BANKING SYSTEM                     
============================================================
  [1] Create New Account
  [2] Login to Account
  [3] Administrative Login
  [4] Exit System
============================================================
 Please select an option (1-4): 
```

### Administrative control dashboard
```text
============================================================
                  SIMPLE BANKING SYSTEM                     
============================================================
 >> ADMIN DASHBOARD CONTROL PANEL
------------------------------------------------------------
  [1] View All Customer Accounts
  [2] Search Customer Account
  [3] Freeze / Unfreeze Account
  [4] Delete Customer Account
  [5] View Security Audit Logs
  [6] View Bank Balance & Statistics
  [7] Logout Administrative Session
============================================================
 Selection (1-7): 
```

---

## 🔬 Learning Outcomes

1.  **Strict Layer Separation**: Minimizing coupling between file system actions and user menus.
2.  **Binary Database Records**: Implementing pointer-seeking updates in C using `fseek` offsets (`SEEK_CUR`, `SEEK_END`).
3.  **Robust Input Handling**: Filtering stack overflows and invalid strings using `fgets` and `sscanf`.
4.  **Transaction Integrity**: Simulating rollback steps in low-level flat file environments.

---

## 📄 License

Licensed under the MIT License. See [LICENSE](file:///c:/Mr-Anonymous-Guy/Mr-Anonymous-Guy/01_C_Projects/Simple_Banking_System/LICENSE) for details.
