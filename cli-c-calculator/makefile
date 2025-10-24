CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c src/operations.c src/scientific.c src/memory.c src/history.c src/utils.c
OUT = build/calculator.exe

all: $(OUT)

$(OUT): $(SRC)
	@if not exist build mkdir build
	$(CC) $(SRC) $(CFLAGS) -o $(OUT) -lm

run: all
	.\$(OUT)

clean:
	@if exist build rmdir /s /q build
