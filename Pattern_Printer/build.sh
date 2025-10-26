#!/bin/bash

# ==============================
# Cross-platform Build Script
# For macOS / Linux / iOS (GCC/Clang)
# ==============================

# Create history folder if missing
if [ ! -d "history" ]; then
    mkdir history
    echo "Created history folder."
fi

# Set executable name
EXE="pattern_printer"

# Compile the program
echo "Compiling Pattern_Printer..."
gcc main.c patterns.c -o $EXE -Wall -Wextra -O2

# Check compilation status
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"

# Run the program
echo "Running program..."
./$EXE

echo
echo "Program finished. Output stored in history/output.txt"
