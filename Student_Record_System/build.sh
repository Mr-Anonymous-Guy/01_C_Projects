#!/bin/bash

# ==========================================
# Student Record System Build & Run Script
# Works on Linux and macOS
# ==========================================

# Colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
NC="\033[0m" # No Color

echo -e "${YELLOW}----------------------------------------${NC}"
echo -e "${GREEN}Building Student Record System...${NC}"
echo -e "${YELLOW}----------------------------------------${NC}"

# Clean previous build
if [ -f student_system ]; then
    rm student_system
fi

# Compile
gcc -Wall -Wextra -Iincludes src/main.c src/student.c -o student_system

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Compilation successful!${NC}"
echo -e "${YELLOW}----------------------------------------${NC}"
echo -e "${GREEN}Running the program...${NC}"
echo -e "${YELLOW}----------------------------------------${NC}"

# Run the executable
./student_system

echo -e "${YELLOW}----------------------------------------${NC}"
echo -e "${GREEN}Program terminated.${NC}"
