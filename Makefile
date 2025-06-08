CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g -Iinclude
SRCDIR   = src
BUILDDIR = build
OUTDIR   = build/out
TARGET   = build/othello

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
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OUTDIR)/%.o)

# Create build directory structure
BUILDDIRS = $(BUILDDIR) \
            $(OUTDIR) \
            $(OUTDIR)/util \
            $(OUTDIR)/datastructure \
            $(OUTDIR)/attribute \
            $(OUTDIR)/ai

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIRS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Generic rule for object files
$(OUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

rebuild: clean all