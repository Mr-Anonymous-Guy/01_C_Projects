#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "utils.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

void ensure_data_directory(void) {
    MKDIR("data");

    // Prepopulate default admin if admins.dat is empty/missing
    FILE *file = fopen(ADMINS_FILE, "rb");
    if (!file) {
        file = fopen(ADMINS_FILE, "wb");
        if (file) {
            Admin default_admin;
            strncpy(default_admin.username, "admin", MAX_ADMIN_USERNAME - 1);
            default_admin.username[MAX_ADMIN_USERNAME - 1] = '\0';
            strncpy(default_admin.password, "admin123", MAX_ADMIN_PASSWORD - 1);
            default_admin.password[MAX_ADMIN_PASSWORD - 1] = '\0';

            fwrite(&default_admin, sizeof(Admin), 1, file);
            fclose(file);
        }
    } else {
        fclose(file);
    }
}

int save_account(const Account *acc) {
    ensure_data_directory();
    FILE *file = fopen(ACCOUNTS_FILE, "r+b");
    if (!file) {
        file = fopen(ACCOUNTS_FILE, "wb");
        if (!file) {
            return 0;
        }
    }

    Account temp;
    int found = 0;

    // Scan to check if record already exists to overwrite it
    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == acc->accountNumber) {
            if (fseek(file, -(long)sizeof(Account), SEEK_CUR) != 0) {
                fclose(file);
                return 0;
            }
            if (fwrite(acc, sizeof(Account), 1, file) != 1) {
                fclose(file);
                return 0;
            }
            found = 1;
            break;
        }
    }

    // Append to end of file if not found
    if (!found) {
        if (fseek(file, 0, SEEK_END) != 0) {
            fclose(file);
            return 0;
        }
        if (fwrite(acc, sizeof(Account), 1, file) != 1) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int load_account(int account_num, Account *acc) {
    ensure_data_directory();
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        return 0;
    }

    Account temp;
    int found = 0;
    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == account_num) {
            *acc = temp;
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

int account_exists(int account_num) {
    Account temp;
    return load_account(account_num, &temp);
}

int delete_account(int account_num) {
    ensure_data_directory();
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) {
        return 0;
    }

    FILE *temp = fopen("data/temp.dat", "wb");
    if (!temp) {
        fclose(file);
        return -1;
    }

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file) == 1) {
        if (acc.accountNumber == account_num) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(Account), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(ACCOUNTS_FILE);
        rename("data/temp.dat", ACCOUNTS_FILE);
        return 1;
    } else {
        remove("data/temp.dat");
        return 0;
    }
}

int load_admin(const char *username, Admin *adm) {
    ensure_data_directory();
    FILE *file = fopen(ADMINS_FILE, "rb");
    if (!file) {
        return 0;
    }

    Admin temp;
    int found = 0;
    while (fread(&temp, sizeof(Admin), 1, file) == 1) {
        if (strcmp(temp.username, username) == 0) {
            *adm = temp;
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

int append_log(const char *file_path, const char *log_entry) {
    ensure_data_directory();
    FILE *file = fopen(file_path, "a");
    if (!file) {
        return 0;
    }
    fprintf(file, "%s", log_entry);
    fclose(file);
    return 1;
}
