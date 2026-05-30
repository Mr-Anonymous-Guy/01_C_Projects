# Security Controls & Safeguards — Simple Banking System

This document outlines the security controls, validation checks, and brute force defenses implemented in the banking core.

---

## 1. Credentials Hashing Algorithm

To comply with database protection standards, plain text user PINs are never stored on disk. Instead, the system applies a one-way hashing formula upon registration and authentication checks:

$$\text{HashedPIN} = (\text{PlainPIN} \times 11) + 17$$

### Flow
1.  User enters 4-digit PIN (e.g. `1234`).
2.  System hashes: $(1234 \times 11) + 17 = 13591$.
3.  The value `13591` is saved to the database.
4.  During login, the input PIN is hashed and checked against the saved value.

---

## 2. Brute-Force Lockout Defense

To prevent dictionary or brute-force PIN attacks:
*   A user is allowed a maximum of **3 consecutive incorrect attempts** (`MAX_LOGIN_RETRIES`) during login.
*   Upon the 3rd failure, the core updates the account status: `isFrozen = 1` (Frozen).
*   The account remains frozen on disk and cannot log in or participate in transfers.
*   The event is recorded in the audit trail:
    `[2026-05-30 19:25:01] ACTION: ACCOUNT_LOCKOUT  | DETAILS: Account 482716 frozen due to too many incorrect login attempts`
*   Only a bank administrator can review and unfreeze the account through the admin dashboard.

---

## 3. Administrative Panel Isolation

*   **Separate Database**: Administrative logins check credentials inside `data/admins.dat`, completely isolated from user accounts in `data/accounts.dat`.
*   **Default Credentials**: Admin credentials are automatically initialized to:
    *   *Username*: `admin`
    *   *Password*: `admin123`
*   **Audit Logging**: Every administrative action (logins, login failures, account freezes, deletions) generates audit records in `data/audit.log` for transparency.
