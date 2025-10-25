# ============================================================
# 🔹 CROSS-PLATFORM AUTO-DETECT MAKEFILE — C/C++ Projects
# Works on Windows, macOS, Linux (and iOS via CLI)
# ============================================================

# --------------------------
# Compiler settings
# --------------------------
CC_C = gcc
CC_CPP = g++
CFLAGS = -Wall -Iinclude
CPPFLAGS = -Wall -Iinclude -std=c++17
LDFLAGS = -lm

# --------------------------
# Detect source files
# --------------------------
SRC_C = $(wildcard src/*.c)
SRC_CPP = $(wildcard src/*.cpp)

# --------------------------
# Determine OS
# --------------------------
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS = LINUX
else ifeq ($(UNAME_S),Darwin)
    OS = MAC
else
    OS = WINDOWS
endif

# --------------------------
# Choose compiler automatically
# --------------------------
ifeq ($(SRC_CPP),)
    LANG = C
    CC = $(CC_C)
    SRC = $(SRC_C)
    OBJ = $(SRC_C:.c=.o)
    FLAGS = $(CFLAGS)
else
    LANG = C++
    CC = $(CC_CPP)
    SRC = $(SRC_CPP)
    OBJ = $(SRC_CPP:.cpp=.o)
    FLAGS = $(CPPFLAGS)
endif

# --------------------------
# Target executable
# --------------------------
ifeq ($(OS),WINDOWS)
    TARGET = build/app.exe
else
    TARGET = build/app
endif

# ============================================================
# BUILD RULES
# ============================================================

all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
ifeq ($(OS),WINDOWS)
	@if not exist build mkdir build
else
	@mkdir -p build
endif
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Build complete! Run with: ./$(TARGET)"

# Compile source files
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

# ============================================================
# CLEANUP
# ============================================================

clean:
ifeq ($(OS),WINDOWS)
	del /Q $(OBJ) 2>nul || true
	if exist $(TARGET) del $(TARGET)
else
	rm -f $(OBJ) $(TARGET)
endif
	@echo "🧹 Cleaned build files!"

rebuild: clean all

run: all
	@echo "🚀 Running program..."
ifeq ($(OS),WINDOWS)
	@$(TARGET)
else
	@./$(TARGET)
endif

help:
	@echo "🔧 Available commands:"
	@echo "   make           → Build the project"
	@echo "   make run       → Build & run"
	@echo "   make clean     → Remove object & build files"
	@echo "   make rebuild   → Clean and rebuild"
	@echo "   make help      → Show this help message"

# ============================================================
# END OF FILE
# ============================================================
