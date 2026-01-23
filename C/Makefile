CC = clang
CFLAGS_COMMON = -Wall -Wextra

# Debug build flags (default)
CFLAGS_DEBUG = $(CFLAGS_COMMON) -g -O0 -DDEBUG

# Release build flags
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2 -DNDEBUG

# Default to debug
CFLAGS = $(CFLAGS_DEBUG)

SRC = src
TESTS = tests

# Raylib configuration
RAYLIB_DIR = raylib
RAYLIB_LIB = $(RAYLIB_DIR)/lib/libraylib.a
RAYLIB_INCLUDE = -I$(RAYLIB_DIR)/include
RAYLIB_LINK = $(RAYLIB_LIB)

# Platform-specific frameworks (macOS)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RAYLIB_FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
else
	RAYLIB_FRAMEWORKS = -lGL -lm -lpthread -ldl -lrt -lX11
endif

.PHONY: all debug release run run-gui test clean raylib

all: debug

# Debug builds
debug: CFLAGS = $(CFLAGS_DEBUG)
debug: game_of_life test_game

# Release builds
release: CFLAGS = $(CFLAGS_RELEASE)
release: game_of_life test_game

# GUI builds (includes raylib dependency)
gui: CFLAGS = $(CFLAGS_DEBUG)
gui: game_gui

gui-release: CFLAGS = $(CFLAGS_RELEASE)
gui-release: game_gui

game_of_life: $(SRC)/game.c $(SRC)/game_core.c $(SRC)/game_core.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/game.c $(SRC)/game_core.c

test_game: $(TESTS)/test_game.c
	$(CC) $(CFLAGS) -o $@ $(TESTS)/test_game.c

game_gui: $(SRC)/game_gui.c $(SRC)/game_core.c $(SRC)/game_core.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/game_gui.c $(SRC)/game_core.c $(RAYLIB_INCLUDE) $(RAYLIB_LINK) $(RAYLIB_FRAMEWORKS)

run: game_of_life
	./game_of_life

run-gui: game_gui
	./game_gui

test: test_game
	./test_game

clean:
	rm -f game_of_life test_game game_gui

clean-all: clean
	cd $(RAYLIB_DIR) && $(MAKE) clean
