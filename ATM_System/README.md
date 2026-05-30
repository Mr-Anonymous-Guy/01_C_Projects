# 💎 ATM Management System in C

A secure, modular, terminal-based Automated Teller Machine (ATM) simulator written in standard C (C99). This project demonstrates modern structured programming practices, separation of concerns, persistent binary storage, safe user-input validation, and transaction auditing.

Developed with a professional software architecture matching real-world systems rather than simple toy-level console applications.

---

## 📂 Project Directory Structure

The project has a strict modular structure to separate logic, headers, persistent data, documentation, and tests:

```text
ATM_System/
├── src/                  # Code implementation modules (.c files)
│   ├── main.c            # Entry point, main application loops
│   ├── auth.c            # Login & PIN verification hashing logic
│   ├── account.c         # Account creation, validation, number generation
│   ├── transaction.c     # Balance management & log parsing
│   ├── file_handler.c    # Low-level file system persistence
│   ├── menu.c            # UI and CLI display elements
│   └── utils.c           # Input security, timestamp helpers, screen controls
├── include/              # Header declarations (.h files)
│   ├── auth.h
│   ├── account.h
│   ├── transaction.h
│   ├── file_handler.h
│   ├── menu.h
│   └── utils.h
├── data/                 # Local persistent files (ignored in Git)
│   ├── accounts.dat      # Encrypted/hashed binary database of accounts
│   └── transactions.log  # Append-only plain text transaction logs
├── docs/                 # System architecture documents
│   ├── architecture.md   # Architectural design details
│   └── flow.md           # Flowcharts & sequencing diagrams
├── tests/                # Testing scenarios
│   └── sample_test_cases.txt # User scenario verification templates
├── Makefile              # GCC build instructions
├── run.sh                # Clean, compile, and run bootstrap bash script
├── run.bat               # Clean, compile, and run bootstrap batch script (Windows)
├── .gitignore            # Git exclusion rules
├── LICENSE               # MIT License
└── README.md             # This documentation
```

---

## ⚡ Tech Stack

*   **Language**: C (C99 standard, fully ANSI-compliant)
*   **Compiler**: GCC (requires standard GCC tools)
*   **Build Automation**: GNU Makefile
*   **Execution**: Bash Shell (`sh`, `bash`)
*   **Storage Subsystem**: Custom flat-file architecture
    *   *Accounts*: Serialized binary blocks (`accounts.dat`)
    *   *Transaction Ledgers*: Append-only logs (`transactions.log`)

---

## 🚀 Key Features

*   **🔒 Secure Hashing Simulation**: Plaintext PIN values are never saved. The system implements a one-way hashing function `H(pin) = pin * 7 + 13` to authenticate accounts.
*   **💾 Database Overwrite Persistence**: Accounts are saved to disk. Balance updates are executed by moving the file pointer dynamically in binary format, allowing the system to update a single record inline without rewriting the database.
*   **⚠️ Safe Input Ingestion**: Utilizes non-blocking line reading to clear input buffers and prevent memory leaks, buffer overflow attacks, or menu infinite loop locks.
*   **🛑 Brute Force Prevention**: Tracks session credentials and triggers a login retry limit lock after 3 unsuccessful attempts.
*   **📊 Transaction Auditing**: Every event (creation, deposits, withdrawals) is stamped with a precise date/time and logged to `data/transactions.log`.
*   **🎨 Clean Terminal UX**: Includes loading progress effects, distinct page partitions, formatting grids, and emojis to simulate a premium console experience.

---

## 🛠️ Build and Run Instructions

### Prerequisites
Ensure you have `gcc` and `make` installed.

#### Linux / macOS
```bash
sudo apt-get install build-essential    # Debian / Ubuntu / WSL
brew install make                       # macOS (via Homebrew)
```

### Quick Run
Execute the bootstrap script. It cleans previous builds, recompiles the modules, and runs the executable automatically.

#### Windows (Command Prompt / PowerShell)
```cmd
run.bat
```

#### Linux / macOS / WSL
```bash
chmod +x run.sh
./run.sh
```

### Manual Compilation
You can compile manually using the provided `Makefile`:

*   **Compile only**:
    ```bash
    make
    ```
*   **Compile and run immediately**:
    ```bash
    make run
    ```
*   **Compile with Debug Symbols (`gdb`)**:
    ```bash
    make debug
    ```
*   **Clean object files and binaries**:
    ```bash
    make clean
    ```
*   **Clean build and wipe user databases**:
    ```bash
    make clean_all
    ```

---

## 🖥️ Example Terminal Output

### Main Dashboard Screen
```text
============================================================
                 💎  ATM MANAGEMENT SYSTEM  💎               
============================================================
 >> Welcome, John Doe | Account: 492716
------------------------------------------------------------
  [1] 💵 Deposit Funds
  [2] 💸 Withdraw Funds
  [3] 🏦 Check Balance
  [4] 📖 View Transaction Ledger
  [5] 🔓 Logout
============================================================
 Please select an option (1-5): 3

============================================================
                 💎  ATM MANAGEMENT SYSTEM  💎               
============================================================
 >> ACCOUNT BALANCE
------------------------------------------------------------
------------------------------------------------------------
  Account Holder  : John Doe
  Account Number  : 492716
  Current Balance : $1250.00
------------------------------------------------------------

Press [Enter] to continue...
```

---

## 📸 Screenshots

*(Add screen captures demonstrating account registration, verification, logs auditing, and CLI menu transitions here)*

---

## 🔬 Learning Outcomes

Developing this system focuses on key computer science concepts:
1.  **Modular C Design**: Managing compiler link steps using header files, double-inclusions protection (`#ifndef`), and static module functions.
2.  **File Stream Pointers**: Learning random file access using `fseek` with `SEEK_CUR` and `ftell` offsets in binary data files.
3.  **Secure Programming**: Building defensive code by sanitizing input streams (`fgets` + `sscanf`), preventing stack pollution, and implementing one-way data hashing.
4.  **Audit Ledgers**: Recording sequential system modifications for tracing and validation.

---

## 🔮 Future Scalability

*   **Database Integration**: Swap binary flat files with SQLite or MySQL connectors.
*   **Network Interface**: Separate the client menu console and file storage into a TCP/IP Client-Server architecture.
*   **GUI Implementation**: Integrate a graphical library like GTK, Raylib, or Qt for C.
*   **Encryption**: Secure the binary file with AES-256 block encryption.

---

## 🤝 Contribution

Contributions are welcome! Please open an issue or submit a pull request with improvements to features, visual styling, or security checks.

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](file:///c:/Mr-Anonymous-Guy/Mr-Anonymous-Guy/01_C_Projects/ATM_System/LICENSE) file for details.
