#ifndef MENU_H
#define MENU_H

#include "account.h"

/**
 * @brief Displays the welcome banner and options.
 */
void display_welcome_menu(void);

/**
 * @brief Displays the dashboard menu for an authenticated user.
 * @param name The name of the user.
 * @param account_num The account number of the user.
 */
void display_main_menu(const char *name, int account_num);

/**
 * @brief Renders the ATM screen header with a custom title.
 * @param title The title text to display in the header.
 */
void print_header(const char *title);

#endif /* MENU_H */
