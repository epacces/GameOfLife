CC = clang
CFLAGS_COMMON = -Wall -Wextra

# Debug build flags (default)
CFLAGS_DEBUG = $(CFLAGS_COMMON) -g -O0 -DDEBUG

# Release build flags
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2 -DNDEBUG

# Default to debug
CFLAGS = $(CFLAGS_DEBUG)

.PHONY: all debug release run test clean

all: debug

# Debug builds
debug: CFLAGS = $(CFLAGS_DEBUG)
debug: game_of_life test_game

# Release builds
release: CFLAGS = $(CFLAGS_RELEASE)
release: game_of_life test_game

game_of_life: game.c
	$(CC) $(CFLAGS) -o $@ $<

test_game: test_game.c
	$(CC) $(CFLAGS) -o $@ $<

run: game_of_life
	./game_of_life

test: test_game
	./test_game

clean:
	rm -f game_of_life test_game
