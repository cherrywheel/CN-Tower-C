# src/GNUmakefile (GNU make, gcc/clang)
# nmake на Windows берёт Makefile.

CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic -O2
CPPFLAGS += -I../include

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: cn_tower_game

cn_tower_game: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c ../include/game.h ../include/dialogues.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f *.o cn_tower_game

.PHONY: all clean
