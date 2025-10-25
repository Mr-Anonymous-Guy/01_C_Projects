#!/bin/bash
# =====================================================
# Cross-Platform Build Script for Linux/macOS/iOS
# =====================================================

# Create build folder if it doesn't exist
mkdir -p build

# Detect C++ or C files
if compgen -G "src/*.cpp" > /dev/null; then
    g++ src/*.cpp -Iinclude -Wall -std=c++17 -o build/app -lm
elif compgen -G "src/*.c" > /dev/null; then
    gcc src/*.c -Iinclude -Wall -o build/app -lm
else
    echo "❌ No source files found in src/ folder!"
    exit 1
fi

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi

# Run the program
echo "🚀 Running program..."
./build/app
