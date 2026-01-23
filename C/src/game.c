#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "game_core.h"

#define ALIVE_CHAR '*'
#define DEAD_CHAR '.'

#define CLEAR_SCREEN printf("\x1b[3J\x1b[H\x1b[2J");
#define REFRESH_RATE_IN_MS 32

void print_grid(CellState *grid) {
    CLEAR_SCREEN
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            printf("%c", get_cell(grid, x, y) == ALIVE ? ALIVE_CHAR : DEAD_CHAR);
        }
        printf("\n");
    }
}

int main(void) {
    srand(time(NULL));

    CellState old_grid[GRID_SIZE], new_grid[GRID_SIZE];
    fill_grid(old_grid, DEAD);
    randomize_grid(old_grid, 2);  // 50% density

    for (;;) {
        compute_new_generation(old_grid, new_grid);
        print_grid(new_grid);
        usleep(REFRESH_RATE_IN_MS * 1000);
        compute_new_generation(new_grid, old_grid);
        print_grid(old_grid);
        usleep(REFRESH_RATE_IN_MS * 1000);
    }
}
