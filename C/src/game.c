#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define GRID_COLS 120
#define GRID_ROWS 120
#define GRID_SIZE (GRID_COLS * GRID_ROWS)
#define ALIVE '*'
#define DEAD '.'

#define CLEAR_SCREEN printf("\x1b[3J\x1b[H\x1b[2J");
#define REFRESH_RATE_IN_MS 32

int pos_to_index(int x, int y) {
  x = (x % GRID_COLS + GRID_COLS) % GRID_COLS;
  y = (y % GRID_ROWS + GRID_ROWS) % GRID_ROWS;
  return y * GRID_COLS + x;
}

void set_cell(char *grid, int x, int y, const char state) {
  grid[pos_to_index(x, y)] = state;
}

char get_cell(const char *grid, const int x, const int y) {
  return grid[pos_to_index(x, y)];
}

void print_grid(char *grid) {
  CLEAR_SCREEN
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      printf("%c", get_cell(grid, x, y));
    }
    printf("\n");
  }
}

void fill_grid(char *grid, char state) {
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      set_cell(grid, x, y, state);
    }
  }
}

int get_alive_neighbors(const char *grid, const int x, const int y) {
  int n_alive = 0;
  for (int y_off = -1; y_off <= 1; y_off++) {
    for (int x_off = -1; x_off <= 1; x_off++) {
      if (x_off || y_off)
        n_alive += get_cell(grid, x + x_off, y + y_off) == ALIVE ? 1 : 0;
    }
  }
  return n_alive;
}

void compute_new_generation(const char *curr_grid, char *next_grid) {
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      char curr_state = get_cell(curr_grid, x, y);
      char new_state = DEAD;
      int alive_count = get_alive_neighbors(curr_grid, x, y);
      if (alive_count == 3)
        new_state = ALIVE;
      if (curr_state == ALIVE && alive_count == 2)
        new_state = ALIVE;
      set_cell(next_grid, x, y, new_state);
    }
  }
}

void randomize_grid(char *grid) {
  srand(time(NULL));
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      if (rand() % 2)
        set_cell(grid, x, y, ALIVE);
    }
  }
}

int main(void) {
  char old_grid[GRID_SIZE], new_grid[GRID_SIZE];
  fill_grid(old_grid, DEAD);
  randomize_grid(old_grid);

  for (;;) {
    compute_new_generation(old_grid, new_grid);
    print_grid(new_grid);
    usleep(REFRESH_RATE_IN_MS * 1000);
    compute_new_generation(new_grid, old_grid);
    print_grid(old_grid);
    usleep(REFRESH_RATE_IN_MS * 1000);
  }
}
