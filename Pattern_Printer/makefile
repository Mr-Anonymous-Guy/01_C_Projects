# Cross-platform Makefile for Pattern Printer

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = pattern_printer

# Source directory
SRC_DIR = src

# Source files
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/patterns.c

# Object files (in the root folder)
OBJ = $(SRC:.c=.o)

# Detect OS
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = $(TARGET).exe
else
    RM = rm -f
    EXE = $(TARGET)
endif

# Default target
all: $(EXE)

# Compile objects
%.o: %.c $(SRC_DIR)/patterns.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Run program
run: $(EXE)
	@echo Running $(EXE)...
ifeq ($(OS),Windows_NT)
	@$(EXE)
else
	@./$(EXE)
endif

# Clean build files
clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(OBJ) $(EXE)
else
	$(RM) $(OBJ) $(EXE)
endif

.PHONY: all run clean
