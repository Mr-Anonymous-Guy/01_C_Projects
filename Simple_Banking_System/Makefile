CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
BIN = $(BUILD_DIR)/bank_system

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

$(BIN): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

ifdef OS
    MKDIR = mkdir
else
    MKDIR = mkdir -p
endif

$(OBJ_DIR):
	-$(MKDIR) $(OBJ_DIR)

$(BUILD_DIR):
	-$(MKDIR) $(BUILD_DIR)

run: all
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

clean_all: clean
	rm -rf data/*.dat data/*.log

debug: CFLAGS += -DDEBUG
debug: all

.PHONY: all run clean clean_all debug
