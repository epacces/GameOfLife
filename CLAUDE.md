# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Multi-language implementation of Conway's Game of Life - a cellular automaton where cells on a 2D grid live/die based on neighbor count. Three parallel implementations demonstrate language-specific idioms.

## Build and Run Commands

### C (Terminal)
```bash
cd C
gcc -o game_of_life game.c
./game_of_life
```

### C (GUI with raylib)
```bash
cd C

# First time: build raylib (only needed once)
cd vendor/raylib/src && make PLATFORM=PLATFORM_DESKTOP && cd ../../..

# Compile game (macOS)
clang -o game_gui game_gui.c -Ivendor/raylib/src -Lvendor/raylib/src -lraylib \
  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

# Compile game (Linux)
gcc -o game_gui game_gui.c -Ivendor/raylib/src -Lvendor/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./game_gui
```

**GUI Controls:**
- SPACE: Pause/Resume simulation
- R: Randomize grid
- C: Clear grid
- Left Mouse: Draw alive cells
- Right Mouse: Erase cells

### Ruby
```bash
cd ruby
ruby game.rb
```

### Swift
```bash
cd swift
swiftc game.swift -o game_of_life
./game_of_life
```

## Testing

### Ruby (RSpec)
```bash
cd ruby
rspec                              # Run all tests
rspec spec/game_of_life_spec.rb    # Run specific test file
rspec --format doc                 # Run with detailed output
```

### C (assertions)
```bash
cd C
gcc -o test_game test_game.c && ./test_game
```

### Swift (XCTest)
```bash
cd swift
swift test                         # Run all tests
swift test --filter PositionTests  # Run specific test class
```

## Architecture

Each implementation is a single self-contained file with consistent structure:

**Core Components:**
- **Grid/Board**: 2D array with toroidal wrapping (edges connect to opposite side)
- **Cell States**: Alive (`*`) and Dead (`.`)

**Key Functions (naming varies by language):**
- `get_cell` / `set_cell` - Cell access with coordinate wrapping
- `count_alive_neighbors` / `get_alive_neighbors` - Count adjacent live cells
- `next_state` / `compute_new_generation` - Apply Game of Life rules
- `draw_board` / `print_grid` / `draw_grid` - Display (terminal or GUI)
- `randomize_state` / `randomize_grid` - Random initialization

**C GUI Version (game_gui.c):**
- Uses raylib for graphical rendering
- 720×720 window (120×120 grid, 6px cells)
- Interactive: pause, randomize, clear, mouse drawing
- Cyan/teal cells on dark background

**Game of Life Rules:**
- Live cell with 2-3 live neighbors survives
- Dead cell with exactly 3 live neighbors becomes alive
- All other cells die or stay dead

## Grid Sizes
- C: 120×120
- Ruby/Swift: 20×90 (rows×cols)
