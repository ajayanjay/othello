CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# temporary makefile
all: 
	$(CC) $(CFLAGS) *.c -o build

run: all
	./build