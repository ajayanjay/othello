# Project: Othello
# Makefile created by Dev-C++ 5.11

CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
OBJ      = build/out/game.o build/out/main.o build/out/replay.o build/out/util/menu.o build/out/util/storage.o build/out/datastructure/array.o build/out/datastructure/deque.o build/out/datastructure/linked.o build/out/datastructure/nbtree.o build/out/datastructure/octuple.o build/out/datastructure/stack.o build/out/attribute/move.o build/out/attribute/player.o build/out/attribute/score.o build/out/ai/ai.o
LINKOBJ  = $(OBJ)
BIN      = build/Othello.exe
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS) 
RM       = rm.exe -f

.PHONY: all all-before all-after clean clean-custom

all: all-before create-build-out-dirs $(BIN) all-after

create-build-out-dirs:
	powershell -Command "if (!(Test-Path -Path 'build')) { New-Item -ItemType Directory -Path 'build' | Out-Null }"
	powershell -Command "if (!(Test-Path -Path 'build/out')) { New-Item -ItemType Directory -Path 'build/out' | Out-Null }"
	powershell -Command "if (!(Test-Path -Path 'build/out/util')) { New-Item -ItemType Directory -Path 'build/out/util' | Out-Null }"
	powershell -Command "if (!(Test-Path -Path 'build/out/datastructure')) { New-Item -ItemType Directory -Path 'build/out/datastructure' | Out-Null }"
	powershell -Command "if (!(Test-Path -Path 'build/out/attribute')) { New-Item -ItemType Directory -Path 'build/out/attribute' | Out-Null }"
	powershell -Command "if (!(Test-Path -Path 'build/out/ai')) { New-Item -ItemType Directory -Path 'build/out/ai' | Out-Null }"

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

build/out/game.o: src/game.c | create-build-out-dirs
	$(CC) -c src/game.c -o build/out/game.o $(CFLAGS)

build/out/main.o: src/main.c | create-build-out-dirs
	$(CC) -c src/main.c -o build/out/main.o $(CFLAGS)

build/out/replay.o: src/replay.c | create-build-out-dirs
	$(CC) -c src/replay.c -o build/out/replay.o $(CFLAGS)

build/out/util/menu.o: src/util/menu.c | create-build-out-dirs
	$(CC) -c src/util/menu.c -o build/out/util/menu.o $(CFLAGS)

build/out/util/storage.o: src/util/storage.c | create-build-out-dirs
	$(CC) -c src/util/storage.c -o build/out/util/storage.o $(CFLAGS)

build/out/datastructure/array.o: src/datastructure/array.c | create-build-out-dirs
	$(CC) -c src/datastructure/array.c -o build/out/datastructure/array.o $(CFLAGS)

build/out/datastructure/deque.o: src/datastructure/deque.c | create-build-out-dirs
	$(CC) -c src/datastructure/deque.c -o build/out/datastructure/deque.o $(CFLAGS)

build/out/datastructure/linked.o: src/datastructure/linked.c | create-build-out-dirs
	$(CC) -c src/datastructure/linked.c -o build/out/datastructure/linked.o $(CFLAGS)

build/out/datastructure/nbtree.o: src/datastructure/nbtree.c | create-build-out-dirs
	$(CC) -c src/datastructure/nbtree.c -o build/out/datastructure/nbtree.o $(CFLAGS)

build/out/datastructure/octuple.o: src/datastructure/octuple.c | create-build-out-dirs
	$(CC) -c src/datastructure/octuple.c -o build/out/datastructure/octuple.o $(CFLAGS)

build/out/datastructure/stack.o: src/datastructure/stack.c | create-build-out-dirs
	$(CC) -c src/datastructure/stack.c -o build/out/datastructure/stack.o $(CFLAGS)

build/out/attribute/move.o: src/attribute/move.c | create-build-out-dirs
	$(CC) -c src/attribute/move.c -o build/out/attribute/move.o $(CFLAGS)

build/out/attribute/player.o: src/attribute/player.c | create-build-out-dirs
	$(CC) -c src/attribute/player.c -o build/out/attribute/player.o $(CFLAGS)

build/out/attribute/score.o: src/attribute/score.c | create-build-out-dirs
	$(CC) -c src/attribute/score.c -o build/out/attribute/score.o $(CFLAGS)

build/out/ai/ai.o: src/ai/ai.c | create-build-out-dirs
	$(CC) -c src/ai/ai.c -o build/out/ai/ai.o $(CFLAGS)
