CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILDDIR = build

# Source files
SRCS = main.c menu.c game.c ai.c score.c player.c
DATASTRUCTURE_SRCS = datastructures/stack.c datastructures/octuple.c datastructures/nbtree.c datastructures/linked.c datastructures/deque.c datastructures/array.c

# Object files in build directory
OBJS = $(addprefix $(BUILDDIR)/, $(SRCS:.c=.o))
DATASTRUCTURE_OBJS = $(addprefix $(BUILDDIR)/, $(DATASTRUCTURE_SRCS:.c=.o))
ALL_OBJS = $(OBJS) $(DATASTRUCTURE_OBJS)

# Target executable
TARGET = $(BUILDDIR)/build

all: $(TARGET)

$(TARGET): $(ALL_OBJS) | $(BUILDDIR)
	$(CC) $(ALL_OBJS) -o $(TARGET)

$(BUILDDIR)/%.o: %.c | $(BUILDDIR) $(BUILDDIR)/datastructures
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/datastructures:
	mkdir -p $(BUILDDIR)/datastructures

clean:
	rm -rf $(BUILDDIR)

run: all
	cd $(BUILDDIR) && ./build