# Application Execution Flow — ATM Management System

This document outlines the user interaction paths, authentication steps, and transaction lifecycles.

---

## 1. User Interaction Flow

This diagram describes the overall loop of the application from start to exit.

```mermaid
graph TD
    start([Start Program]) --> dirCheck[Ensure data/ directory exists]
    dirCheck --> welcomeMenu{Welcome Menu}
    
    welcomeMenu -- 1: Create Account --> createAcc[Prompt: Name, PIN, Initial Deposit]
    createAcc --> genAccNo[Generate Unique 6-digit Account Number]
    genAccNo --> saveDisk[Save Account binary to accounts.dat]
    saveDisk --> logCreate[Append Create event to transactions.log]
    logCreate --> showAccDetails[Display generated Account Number]
    showAccDetails --> welcomeMenu
    
    welcomeMenu -- 2: Login --> promptAccNo[Prompt: Account Number]
    promptAccNo --> checkExist{Account Exists?}
    checkExist -- No --> errNoAcc[Display: Account does not exist] --> welcomeMenu
    checkExist -- Yes --> authLoop[Prompt PIN - Max 3 attempts]
    authLoop -- Success --> loadSession[Load Account Details to Session]
    loadSession --> dashMenu{Dashboard Menu}
    authLoop -- Failure --> lockUser[Display: Login attempts exceeded] --> welcomeMenu
    
    dashMenu -- 1: Deposit --> depFlow[Deposit Funds Flow] --> dashMenu
    dashMenu -- 2: Withdraw --> wdrFlow[Withdrawal Funds Flow] --> dashMenu
    dashMenu -- 3: Balance --> showBal[Display Current Balance] --> dashMenu
    dashMenu -- 4: History --> printHist[Parse & Print logs for Account] --> dashMenu
    dashMenu -- 5: Logout --> clearSession[Clear Session State] --> welcomeMenu
    
    welcomeMenu -- 3: Exit --> stop([End Program])
```

---

## 2. Authentication Flow

When a user attempts to login, the system processes details sequentially:

```mermaid
sequenceDiagram
    actor User
    participant Main as main.c
    participant Auth as auth.c
    participant FH as file_handler.c

    User->>Main: Input 6-Digit Account Number
    Main->>FH: account_exists(account_num)
    FH-->>Main: Returns Exist Status (True/False)
    
    Note over Main,User: If False, Login terminates immediately
    
    loop Max 3 attempts
        User->>Main: Input 4-Digit PIN
        Main->>Auth: authenticate_user(account_num, pin, &active_acc)
        Auth->>FH: load_account(account_num, &temp)
        FH-->>Auth: Loaded account data
        Auth->>Auth: hash_pin(input_pin)
        Auth->>Auth: Compare input hash with temp.hashedPin
        
        alt PIN Matches
            Auth-->>Main: Returns 1 (Authenticated)
            Note over Main,User: Enter Session Dashboard
        else PIN Mismatches
            Auth-->>Main: Returns 0 (Incorrect PIN)
            Main-->>User: Display Incorrect PIN and attempt count
        end
    end
    
    Note over Main,User: If 3 unsuccessful attempts, terminate login
```

---

## 3. Transaction Flow

When performing deposits or withdrawals, the transaction engine maintains data consistency on disk:

```mermaid
graph TD
    startTrans([Initiate Transaction]) --> checkType{Type?}
    
    checkType -- Deposit --> valDep{Amount > 0?}
    valDep -- No --> errAmt[Return Error: Invalid Amount]
    valDep -- Yes --> applyDep[In-Memory: Add to activeAccount.balance]
    applyDep --> saveDepDisk{save_account() to disk}
    saveDepDisk -- Success --> logDep[Append DEPOSIT log to transactions.log]
    logDep --> successDep([Return Success])
    saveDepDisk -- Fail --> rollbackDep[Rollback: Subtract amount in-memory] --> failDisk([Return Error: Disk Write Failed])

    checkType -- Withdraw --> valWdr{Amount > 0?}
    valWdr -- No --> errAmt
    valWdr -- Yes --> checkBal{balance >= Amount?}
    checkBal -- No --> errFunds[Return Error: Insufficient Funds]
    checkBal -- Yes --> applyWdr[In-Memory: Subtract from activeAccount.balance]
    applyWdr --> saveWdrDisk{save_account() to disk}
    saveWdrDisk -- Success --> logWdr[Append WITHDRAW log to transactions.log]
    logWdr --> successWdr([Return Success])
    saveWdrDisk -- Fail --> rollbackWdr[Rollback: Add amount in-memory] --> failDisk
```
