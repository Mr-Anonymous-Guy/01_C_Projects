# Cross-platform Makefile for Pattern Printer

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = pattern_printer

# Source files
SRC = src/main.c src/Patterns.c

# Object files
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = $(TARGET).exe
    CLEAN_OBJ = $(subst /,\,$(OBJ))
else
    RM = rm -f
    EXE = $(TARGET)
    CLEAN_OBJ = $(OBJ)
endif

# Default target
all: $(EXE)

# Compile objects
%.o: %.c src/patterns.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Run program
run: $(EXE)
	@echo Running $(EXE)...
ifeq ($(OS),Windows_NT)
	.\$(EXE)
else
	./$(EXE)
endif

# Clean build files
clean:
	-$(RM) $(CLEAN_OBJ) $(EXE) 2>nul || true

.PHONY: all run clean
