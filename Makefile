CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILDDIR = build # placeholder for build directory

# temporary makefile
all: 
	$(CC) $(CFLAGS) datastructures/*.c *.c -o build

run: all
	./build