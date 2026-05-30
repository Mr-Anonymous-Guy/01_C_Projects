CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L
DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS = -O3

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/novashell

.PHONY: all clean run debug release test sanitize

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

release: CFLAGS += $(RELEASE_FLAGS)
release: all

sanitize: CFLAGS += $(DEBUG_FLAGS)
sanitize: all

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

test: all
	# To be implemented
	@echo "Running tests..."
