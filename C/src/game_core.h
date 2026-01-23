#ifndef GAME_CORE_H
#define GAME_CORE_H

#define GRID_COLS 120
#define GRID_ROWS 120
#define GRID_SIZE (GRID_COLS * GRID_ROWS)

typedef enum { DEAD = 0, ALIVE = 1 } CellState;

int pos_to_index(int x, int y);
void set_cell(CellState *grid, int x, int y, CellState state);
CellState get_cell(const CellState *grid, int x, int y);
void fill_grid(CellState *grid, CellState state);
int get_alive_neighbors(const CellState *grid, int x, int y);
void compute_new_generation(const CellState *curr_grid, CellState *next_grid);
void randomize_grid(CellState *grid, int density_inverse);

#endif
