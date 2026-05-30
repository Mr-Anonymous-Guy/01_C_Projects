#ifndef MENU_H
#define MENU_H

/**
 * @brief Displays the banking system top-level welcome menu options.
 */
void display_welcome_menu(void);

/**
 * @brief Displays the main dashboard menu choices for logged-in users.
 * @param name The user's name.
 * @param account_num The user's account number.
 */
void display_main_menu(const char *name, int account_num);

/**
 * @brief Prints a standardized header title block.
 * @param title The title text.
 */
void print_header(const char *title);

#endif /* MENU_H */
