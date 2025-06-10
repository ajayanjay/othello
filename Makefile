# Detect OS
ifeq ($(OS),Windows_NT)
    RM = del /Q /S
    MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    EXEEXT = .exe
    NULLDEV = NUL
else
    RM = rm -rf
    MKDIR = mkdir -p $(1)
    EXEEXT =
    NULLDEV = /dev/null
endif

CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -O2 -Iinclude
SRCDIR   = src
BUILDDIR = build
OBJDIR   = $(BUILDDIR)/out
TARGET   = $(BUILDDIR)/othello$(EXEEXT)

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
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

.PHONY: all clean rebuild directories run

all: directories $(TARGET)

directories:
	@$(call MKDIR, $(OBJDIR))

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(call MKDIR, $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(subst /,\,$(BUILDDIR)) $(RM) $(subst /,\,$(BUILDDIR))\* > $(NULLDEV) 2>&1 && rmdir /S /Q $(subst /,\,$(BUILDDIR))
else
	$(RM) $(BUILDDIR)
endif

run: all
	$(TARGET)

rebuild: clean all
