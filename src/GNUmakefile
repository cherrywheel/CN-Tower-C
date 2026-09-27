# src/GNUmakefile for gnu make with gcc or clang
# nmake on windows uses Makefile instead

CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic -O2
CPPFLAGS += -I../include

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: cn_tower_game

cn_tower_game: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c ../include/game.h ../include/dialogues.h ../include/ui.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f *.o cn_tower_game

.PHONY: all clean
