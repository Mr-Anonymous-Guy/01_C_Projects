#include <stdio.h>
#include "utils.h"
#include "operations.h"
#include "scientific.h"
#include "memory.h"
#include "history.h"

// --- Scientific operations submenu ---
void scientific_menu() {
    int choice;
    float num, num2, result;
    int n;
    char buffer[100];

    while(1) {
        print_header("Scientific Operations");
        printf("1. Power (x^y)\n2. Square Root\n3. Factorial\n");
        printf("4. Sine\n5. Cosine\n6. Tangent\n7. Logarithm\n8. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                num = get_number("Base: ");
                num2 = get_number("Exponent: ");
                result = power(num, num2);
                print_result(result);
                snprintf(buffer, 100, "Power %.2f^%.2f = %.2f", num, num2, result);
                add_to_history(buffer);
                break;
            case 2:
                num = get_number("Number: ");
                result = square_root(num);
                print_result(result);
                snprintf(buffer, 100, "Sqrt %.2f = %.2f", num, result);
                add_to_history(buffer);
                break;
            case 3:
                n = (int)get_number("Number: ");
                result = factorial(n);
                print_result(result);
                snprintf(buffer, 100, "Factorial %d! = %.0f", n, result);
                add_to_history(buffer);
                break;
            case 4:
                num = get_number("Angle in radians: ");
                result = sine(num);
                print_result(result);
                snprintf(buffer, 100, "Sin(%.2f) = %.2f", num, result);
                add_to_history(buffer);
                break;
            case 5:
                num = get_number("Angle in radians: ");
                result = cosine(num);
                print_result(result);
                snprintf(buffer, 100, "Cos(%.2f) = %.2f", num, result);
                add_to_history(buffer);
                break;
            case 6:
                num = get_number("Angle in radians: ");
                result = tangent(num);
                print_result(result);
                snprintf(buffer, 100, "Tan(%.2f) = %.2f", num, result);
                add_to_history(buffer);
                break;
            case 7:
                num = get_number("Number: ");
                result = logarithm(num);
                print_result(result);
                snprintf(buffer, 100, "Log(%.2f) = %.2f", num, result);
                add_to_history(buffer);
                break;
            case 8:
                return; // Back to main menu
            default:
                printf("\033[1;31mInvalid choice! Try again.\033[0m\n");
        }
    }
}

// --- Memory operations submenu ---
void memory_menu() {
    int choice;
    float num;
    char buffer[100];

    while(1) {
        print_header("Memory Operations");
        printf("1. M+ (Add to Memory)\n2. M- (Subtract from Memory)\n");
        printf("3. MR (Recall Memory)\n4. MC (Clear Memory)\n5. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                num = get_number("Add value to memory: ");
                memory_add(num);
                printf("\033[1;32mAdded %.2f to memory.\033[0m\n", num);
                snprintf(buffer, 100, "Memory Add %.2f", num);
                add_to_history(buffer);
                break;
            case 2:
                num = get_number("Subtract value from memory: ");
                memory_subtract(num);
                printf("\033[1;32mSubtracted %.2f from memory.\033[0m\n", num);
                snprintf(buffer, 100, "Memory Subtract %.2f", num);
                add_to_history(buffer);
                break;
            case 3:
                num = memory_recall();
                printf("\033[1;36mMemory Recall: %.2f\033[0m\n", num);
                snprintf(buffer, 100, "Memory Recall %.2f", num);
                add_to_history(buffer);
                break;
            case 4:
                memory_clear();
                printf("\033[1;33mMemory Cleared.\033[0m\n");
                add_to_history("Memory Cleared");
                break;
            case 5:
                return; // Back to main menu
            default:
                printf("\033[1;31mInvalid choice! Try again.\033[0m\n");
        }
    }
}

// --- Main menu ---
int main() {
    char choice[3];
    float num1, num2, result;
    char buffer[100];

    while(1) {
        print_header("CLI C Calculator");
        printf("1. Addition (a)\n2. Subtraction (s)\n3. Multiplication (m)\n");
        printf("4. Division (d)\n5. Scientific (S)\n6. Memory (M)\n7. History (h)\n");
        printf("8. Clear Screen (c)\n9. Exit (e)\n");
        printf("Enter choice: ");
        scanf("%2s", choice);

        if(choice[0]=='1' || choice[0]=='a') {
            num1 = get_number("First number: ");
            num2 = get_number("Second number: ");
            result = add(num1, num2);
            print_result(result);
            snprintf(buffer, 100, "Add %.2f + %.2f = %.2f", num1, num2, result);
            add_to_history(buffer);
        } 
        else if(choice[0]=='2' || choice[0]=='s') {
            num1 = get_number("First number: ");
            num2 = get_number("Second number: ");
            result = subtract(num1, num2);
            print_result(result);
            snprintf(buffer, 100, "Subtract %.2f - %.2f = %.2f", num1, num2, result);
            add_to_history(buffer);
        } 
        else if(choice[0]=='3' || choice[0]=='m') {
            num1 = get_number("First number: ");
            num2 = get_number("Second number: ");
            result = multiply(num1, num2);
            print_result(result);
            snprintf(buffer, 100, "Multiply %.2f * %.2f = %.2f", num1, num2, result);
            add_to_history(buffer);
        } 
        else if(choice[0]=='4' || choice[0]=='d') {
            num1 = get_number("First number: ");
            num2 = get_number("Second number: ");
            result = divide(num1, num2);
            print_result(result);
            snprintf(buffer, 100, "Divide %.2f / %.2f = %.2f", num1, num2, result);
            add_to_history(buffer);
        } 
        else if(choice[0]=='5' || choice[0]=='S') {
            scientific_menu();
        } 
        else if(choice[0]=='6' || choice[0]=='M') {
            memory_menu();
        } 
        else if(choice[0]=='7' || choice[0]=='h') {
            display_history();
        } 
        else if(choice[0]=='8' || choice[0]=='c') {
            clear_screen();
        } 
        else if(choice[0]=='9' || choice[0]=='e') {
            printf("Exiting...\n");
            break;
        } 
        else {
            printf("\033[1;31mInvalid choice! Try again.\033[0m\n");
        }
    }

    return 0;
}
