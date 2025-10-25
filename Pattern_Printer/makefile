# Cross-platform Makefile for Pattern Printer

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = pattern_printer

# Source files
SRC = main.c patterns.c

# Object files
OBJ = $(SRC:.c=.o)

# Detect OS (Windows vs Unix)
UNAME_S := $(shell uname -s)

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
%.o: %.c patterns.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Run program
run: $(EXE)
	@echo Running $(EXE)...
ifeq ($(OS),Windows_NT)
	$(EXE)
else
	./$(EXE)
endif

# Clean build files
clean:
	$(RM) $(OBJ) $(EXE)

.PHONY: all run clean
