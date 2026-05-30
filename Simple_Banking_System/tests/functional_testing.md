# Functional Testing & QA Log — Simple Banking System

This document outlines the testing parameters, validation logs, and unit checklists used to verify the functional specifications of the Simple Banking System.

---

## 1. QA Checklist Matrix

| Module | Test Case Description | Test Input / Inputs | Expected Output / Checks | Status |
| :--- | :--- | :--- | :--- | :--- |
| **Auth** | Wrong PIN increments retries | Acc: `100001`, PIN: `9999` | Attempt counter increases; lockout after 3 fails. | Pass |
| **Auth** | Block frozen account log | Acc: `100001` (Frozen) | "This account is frozen. Contact administrator." | Pass |
| **Account** | Block special chars in Name | Name: `John_Doe#` | "Invalid Name format." | Pass |
| **Account** | Auto-generate Acc ID | Initial details | Sequential random ID verification (unique 6-digit). | Pass |
| **Transaction** | Deposit increases balance | Deposit: `$250.00` | In-memory and file records update + log entry. | Pass |
| **Transaction** | Withdraw overdraft block | Withdraw: `$2000` (Bal: 500) | Blocked. residual balance unchanged. | Pass |
| **Transaction** | Transfer balance integrity | Transfer: `$100` | Sender: -$100, Receiver: +$100. Consistency lock. | Pass |
| **Admin** | Unfreeze frozen users | Frozen Acc Number | Status changes to active. User can log back in. | Pass |
| **Admin** | Wipe deleted accounts | Deleted Acc Number | Record missing from file; list update. | Pass |
| **Logger** | Append audit trails | Freeze action | Log line written to `audit.log` with timestamp. | Pass |

---

## 2. Input Sanitization Assertions

*   **Keyboard Ingestion**:
    The system reads lines using `fgets` to prevent buffer overflow attacks, then converts them using `sscanf` with extra char match controls. Standard `scanf` string bounds are not used.
*   **Whitespaces Trim**:
    Leading and trailing spaces in name registrations are cleaned before saving to prevent records like `"   John Doe   "`.
*   **Floating-point precision checks**:
    Values are processed in double precision. Negative deposit or withdrawal inputs are intercepted before transaction engines call database files.
