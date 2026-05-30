#!/bin/bash

# Clear CLI screen
clear

echo "============================================================"
echo "         💎  ATM MANAGEMENT SYSTEM BUILD & RUN  💎"
echo "============================================================"

# Step 1: Clean existing build files
echo " [*] Cleaning old object files and binaries..."
make clean

# Step 2: Compile the application
echo " [*] Compiling code modules..."
make

if [ $? -eq 0 ]; then
    echo " [*] Compilation successful! Starting the application..."
    echo "============================================================"
    # Step 3: Run the binary
    ./atm_system
else
    echo " [ERROR] Compilation failed. Please resolve the errors above."
    exit 1
fi
