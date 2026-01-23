#include "game_core.h"
#include <stdlib.h>

int pos_to_index(int x, int y) {
    x = (x % GRID_COLS + GRID_COLS) % GRID_COLS;
    y = (y % GRID_ROWS + GRID_ROWS) % GRID_ROWS;
    return y * GRID_COLS + x;
}

void set_cell(CellState *grid, int x, int y, CellState state) {
    grid[pos_to_index(x, y)] = state;
}

CellState get_cell(const CellState *grid, int x, int y) {
    return grid[pos_to_index(x, y)];
}

void fill_grid(CellState *grid, CellState state) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            set_cell(grid, x, y, state);
        }
    }
}

int get_alive_neighbors(const CellState *grid, int x, int y) {
    int n_alive = 0;
    for (int y_off = -1; y_off <= 1; y_off++) {
        for (int x_off = -1; x_off <= 1; x_off++) {
            if (x_off || y_off) {
                n_alive += get_cell(grid, x + x_off, y + y_off) == ALIVE ? 1 : 0;
            }
        }
    }
    return n_alive;
}

void compute_new_generation(const CellState *curr_grid, CellState *next_grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            CellState curr_state = get_cell(curr_grid, x, y);
            CellState new_state = DEAD;
            int alive_count = get_alive_neighbors(curr_grid, x, y);
            if (alive_count == 3)
                new_state = ALIVE;
            if (curr_state == ALIVE && alive_count == 2)
                new_state = ALIVE;
            set_cell(next_grid, x, y, new_state);
        }
    }
}

void randomize_grid(CellState *grid, int density_inverse) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (rand() % density_inverse == 0)
                set_cell(grid, x, y, ALIVE);
        }
    }
}
