#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Clears the CLI screen in a cross-platform manner.
 */
void clear_screen(void);

/**
 * @brief Prints a line separator of a specified character and length.
 * @param ch Character to print.
 * @param len Repeat count.
 */
void print_separator(char ch, int len);

/**
 * @brief Gets the current date and time formatted as YYYY-MM-DD HH:MM:SS.
 * @param buffer Output buffer.
 * @param max_len Buffer size.
 */
void get_timestamp(char *buffer, int max_len);

/**
 * @brief Safely reads an integer from standard input, filtering buffer overflows.
 * @param val Pointer to output integer.
 * @return int 1 if successful, 0 if invalid input.
 */
int read_int(int *val);

/**
 * @brief Safely reads a double-precision float from standard input.
 * @param val Pointer to output double.
 * @return int 1 if successful, 0 if invalid.
 */
int read_double(double *val);

/**
 * @brief Safely reads a string from stdin, removing leading/trailing spaces and newlines.
 * @param buffer Output buffer.
 * @param max_len Buffer size.
 */
void read_string(char *buffer, int max_len);

/**
 * @brief Renders a console loading animation.
 * @param message Message to display.
 */
void show_loading_effect(const char *message);

/**
 * @brief Blocks execution until the user presses Enter.
 */
void press_enter_to_continue(void);

/**
 * @brief Sleeps for a specified amount of milliseconds.
 * @param milliseconds Time to sleep.
 */
void sleep_ms(int milliseconds);

#endif /* UTILS_H */
