CC = gcc
CFLAGS = -Wall -Wextra -Iincludes
SRC = src/main.c src/student.c
OUT = student_system.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	del $(OUT)
