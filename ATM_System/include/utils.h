#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Clears the terminal screen in a cross-platform manner.
 */
void clear_screen(void);

/**
 * @brief Prints a line separator of a specified character and length.
 * @param ch The character to repeat.
 * @param len The number of times to print the character.
 */
void print_separator(char ch, int len);

/**
 * @brief Retrieves the current timestamp in a formatted string (YYYY-MM-DD HH:MM:SS).
 * @param buffer Buffer to write the timestamp to.
 * @param max_len Size of the buffer.
 */
void get_timestamp(char *buffer, int max_len);

/**
 * @brief Safely reads an integer from stdin, clearing any invalid input buffer.
 * @param val Pointer to the integer variable to store the input.
 * @return int 1 if successful, 0 if invalid input.
 */
int read_int(int *val);

/**
 * @brief Safely reads a float from stdin, clearing any invalid input buffer.
 * @param val Pointer to the float variable to store the input.
 * @return int 1 if successful, 0 if invalid input.
 */
int read_float(float *val);

/**
 * @brief Safely reads a string from stdin, stripping trailing newlines and removing leading/trailing spaces.
 * @param buffer Buffer to write the string to.
 * @param max_len Maximum length to read.
 */
void read_string(char *buffer, int max_len);

/**
 * @brief Simulates a premium CLI loading effect with a message.
 * @param message The text to display next to the loading effect.
 */
void show_loading_effect(const char *message);

/**
 * @brief Pauses execution and prompts the user to press Enter to continue.
 */
void press_enter_to_continue(void);

/**
 * @brief Simulates a delay in milliseconds.
 * @param milliseconds The duration of the delay.
 */
void sleep_ms(int milliseconds);

#endif /* UTILS_H */
