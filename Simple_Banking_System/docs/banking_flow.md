# Core Banking Operations Flow — Simple Banking System

This document outlines the logical flows and validation steps for key banking events.

---

## 1. Secure Authentication Flow

```mermaid
graph TD
    start([Login Request]) --> input[Input Account Number]
    input --> checkAcc{Account Exists?}
    checkAcc -- No --> errNo[Show: Account does not exist]
    checkAcc -- Yes --> checkFrz{Account Frozen?}
    checkFrz -- Yes --> errFrz[Show: Account is frozen]
    checkFrz -- No --> promptPIN[Prompt PIN - Retries remaining]
    promptPIN --> checkPIN{PIN Matches?}
    checkPIN -- Yes --> success[Enter Session Dashboard]
    checkPIN -- No --> incRetry[Increment retries count]
    incRetry --> checkLimit{Retries >= 3?}
    checkLimit -- Yes --> lock[Set Account to Frozen on Disk] --> logAudit[Write Audit Log] --> stop([Terminated])
    checkLimit -- No --> promptPIN
```

---

## 2. Double-Entry Transfer Flow (Transactional Rollback)

To ensure data integrity, transfers utilize transactional rollback steps to handle disk write errors:

```mermaid
sequenceDiagram
    participant Tx as transaction.c
    participant Sender as Sender Record (Disk)
    participant Receiver as Receiver Record (Disk)

    Note over Tx: Validate: Amount > 0, sender balance >= amount, accounts not frozen
    Tx->>Tx: In-Memory: Deduct from Sender balance
    Tx->>Tx: In-Memory: Add to Receiver balance
    
    Tx->>Sender: Save updated Sender state
    alt Sender save fails
        Tx->>Tx: Revert in-memory changes
        Note over Tx: Transaction failed (Disk Error)
    else Sender save succeeds
        Tx->>Receiver: Save updated Receiver state
        alt Receiver save fails
            Tx->>Tx: In-Memory: Revert Sender changes
            Tx->>Sender: Save reverted Sender state to disk
            Note over Tx: Transaction rolled back (Disk Error)
        else Receiver save succeeds
            Note over Tx: Transaction committed (SUCCESS)
        end
    end
```
