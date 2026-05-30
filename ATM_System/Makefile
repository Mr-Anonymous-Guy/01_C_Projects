CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN = atm_system

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default rule
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: all
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

clean_all: clean
	rm -rf data/*.dat data/*.log

debug: CFLAGS += -g
debug: all

.PHONY: all run clean clean_all debug
