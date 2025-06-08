CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g -Iinclude
SRCDIR   = src
BUILDDIR = build
TARGET   = othello

# Source files
SOURCES = $(SRCDIR)/game.c \
          $(SRCDIR)/main.c \
          $(SRCDIR)/replay.c \
          $(SRCDIR)/util/menu.c \
          $(SRCDIR)/util/storage.c \
          $(SRCDIR)/datastructure/array.c \
          $(SRCDIR)/datastructure/deque.c \
          $(SRCDIR)/datastructure/linked.c \
          $(SRCDIR)/datastructure/nbtree.c \
          $(SRCDIR)/datastructure/octuple.c \
          $(SRCDIR)/datastructure/stack.c \
          $(SRCDIR)/attribute/move.c \
          $(SRCDIR)/attribute/player.c \
          $(SRCDIR)/attribute/score.c \
          $(SRCDIR)/ai/ai.c \
          $(SRCDIR)/attribute/piece.c

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Create build directory structure
BUILDDIRS = $(BUILDDIR) \
            $(BUILDDIR)/util \
            $(BUILDDIR)/datastructure \
            $(BUILDDIR)/attribute \
            $(BUILDDIR)/ai

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIRS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Generic rule for object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

rebuild: clean all