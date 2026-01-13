/*
 * Tests for Game of Life - C implementation
 * Compile: gcc -o test_game test_game.c
 * Run: ./test_game
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Use smaller grid for testing */
#define GRID_COLS 5
#define GRID_ROWS 5
#define GRID_SIZE (GRID_COLS * GRID_ROWS)
#define ALIVE '*'
#define DEAD '.'

/* Core functions (same logic as game.c) */
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

void fill_grid(char *grid, char state) {
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i] = state;
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

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) void name(void)
#define RUN_TEST(name) do { \
    printf("  %s... ", #name); \
    tests_run++; \
    name(); \
    tests_passed++; \
    printf("PASSED\n"); \
} while(0)

/* Tests for pos_to_index */
TEST(test_pos_to_index_basic) {
    assert(pos_to_index(0, 0) == 0);
    assert(pos_to_index(1, 0) == 1);
    assert(pos_to_index(0, 1) == GRID_COLS);
    assert(pos_to_index(2, 2) == 2 * GRID_COLS + 2);
}

TEST(test_pos_to_index_wrapping_positive) {
    /* x wraps around */
    assert(pos_to_index(GRID_COLS, 0) == pos_to_index(0, 0));
    assert(pos_to_index(GRID_COLS + 1, 0) == pos_to_index(1, 0));
    /* y wraps around */
    assert(pos_to_index(0, GRID_ROWS) == pos_to_index(0, 0));
    assert(pos_to_index(0, GRID_ROWS + 1) == pos_to_index(0, 1));
}

TEST(test_pos_to_index_wrapping_negative) {
    /* negative x wraps */
    assert(pos_to_index(-1, 0) == pos_to_index(GRID_COLS - 1, 0));
    assert(pos_to_index(-2, 0) == pos_to_index(GRID_COLS - 2, 0));
    /* negative y wraps */
    assert(pos_to_index(0, -1) == pos_to_index(0, GRID_ROWS - 1));
    assert(pos_to_index(0, -2) == pos_to_index(0, GRID_ROWS - 2));
}

/* Tests for get_cell and set_cell */
TEST(test_set_and_get_cell) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    set_cell(grid, 1, 2, ALIVE);
    assert(get_cell(grid, 1, 2) == ALIVE);
    assert(get_cell(grid, 0, 0) == DEAD);
}

TEST(test_set_cell_with_wrapping) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Set cell using wrapped coordinates */
    set_cell(grid, -1, -1, ALIVE);
    assert(get_cell(grid, GRID_COLS - 1, GRID_ROWS - 1) == ALIVE);

    set_cell(grid, GRID_COLS, GRID_ROWS, ALIVE);
    assert(get_cell(grid, 0, 0) == ALIVE);
}

/* Tests for get_alive_neighbors */
TEST(test_alive_neighbors_none) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    assert(get_alive_neighbors(grid, 2, 2) == 0);
}

TEST(test_alive_neighbors_all_eight) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Surround center cell (2,2) with alive cells */
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx || dy) {
                set_cell(grid, 2 + dx, 2 + dy, ALIVE);
            }
        }
    }
    assert(get_alive_neighbors(grid, 2, 2) == 8);
}

TEST(test_alive_neighbors_does_not_count_self) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Only the cell itself is alive */
    set_cell(grid, 2, 2, ALIVE);
    assert(get_alive_neighbors(grid, 2, 2) == 0);
}

TEST(test_alive_neighbors_with_wrapping) {
    char grid[GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Cell at corner (0,0), place alive cells at wrapped positions */
    set_cell(grid, GRID_COLS - 1, GRID_ROWS - 1, ALIVE);  /* wraps to top-left diagonal */
    set_cell(grid, 1, 0, ALIVE);
    set_cell(grid, 0, 1, ALIVE);

    assert(get_alive_neighbors(grid, 0, 0) == 3);
}

/* Tests for Game of Life rules via compute_new_generation */
TEST(test_blinker_oscillator) {
    char grid[GRID_SIZE], next[GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* Vertical blinker at column 2: (2,1), (2,2), (2,3) */
    set_cell(grid, 2, 1, ALIVE);
    set_cell(grid, 2, 2, ALIVE);
    set_cell(grid, 2, 3, ALIVE);

    compute_new_generation(grid, next);

    /* Should become horizontal at row 2: (1,2), (2,2), (3,2) */
    assert(get_cell(next, 1, 2) == ALIVE);
    assert(get_cell(next, 2, 2) == ALIVE);
    assert(get_cell(next, 3, 2) == ALIVE);

    /* Original vertical cells (except center) should be dead */
    assert(get_cell(next, 2, 1) == DEAD);
    assert(get_cell(next, 2, 3) == DEAD);
}

TEST(test_block_still_life) {
    char grid[GRID_SIZE], next[GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* 2x2 block at (1,1), (1,2), (2,1), (2,2) */
    set_cell(grid, 1, 1, ALIVE);
    set_cell(grid, 1, 2, ALIVE);
    set_cell(grid, 2, 1, ALIVE);
    set_cell(grid, 2, 2, ALIVE);

    compute_new_generation(grid, next);

    /* Block should remain unchanged */
    assert(get_cell(next, 1, 1) == ALIVE);
    assert(get_cell(next, 1, 2) == ALIVE);
    assert(get_cell(next, 2, 1) == ALIVE);
    assert(get_cell(next, 2, 2) == ALIVE);

    /* Surrounding cells should still be dead */
    assert(get_cell(next, 0, 0) == DEAD);
    assert(get_cell(next, 0, 1) == DEAD);
    assert(get_cell(next, 3, 3) == DEAD);
}

TEST(test_underpopulation) {
    char grid[GRID_SIZE], next[GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* Single cell dies (0 neighbors) */
    set_cell(grid, 2, 2, ALIVE);
    compute_new_generation(grid, next);
    assert(get_cell(next, 2, 2) == DEAD);
}

TEST(test_overpopulation) {
    char grid[GRID_SIZE], next[GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* Center cell with 4+ neighbors dies */
    set_cell(grid, 2, 2, ALIVE);
    set_cell(grid, 1, 1, ALIVE);
    set_cell(grid, 1, 2, ALIVE);
    set_cell(grid, 1, 3, ALIVE);
    set_cell(grid, 2, 1, ALIVE);

    compute_new_generation(grid, next);
    assert(get_cell(next, 2, 2) == DEAD);
}

TEST(test_reproduction) {
    char grid[GRID_SIZE], next[GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* Dead cell with exactly 3 neighbors becomes alive */
    set_cell(grid, 1, 2, ALIVE);
    set_cell(grid, 2, 1, ALIVE);
    set_cell(grid, 3, 2, ALIVE);

    compute_new_generation(grid, next);
    assert(get_cell(next, 2, 2) == ALIVE);
}

int main(void) {
    printf("Running Game of Life tests (C)...\n\n");

    printf("pos_to_index tests:\n");
    RUN_TEST(test_pos_to_index_basic);
    RUN_TEST(test_pos_to_index_wrapping_positive);
    RUN_TEST(test_pos_to_index_wrapping_negative);

    printf("\nCell get/set tests:\n");
    RUN_TEST(test_set_and_get_cell);
    RUN_TEST(test_set_cell_with_wrapping);

    printf("\nNeighbor counting tests:\n");
    RUN_TEST(test_alive_neighbors_none);
    RUN_TEST(test_alive_neighbors_all_eight);
    RUN_TEST(test_alive_neighbors_does_not_count_self);
    RUN_TEST(test_alive_neighbors_with_wrapping);

    printf("\nGame of Life rules tests:\n");
    RUN_TEST(test_blinker_oscillator);
    RUN_TEST(test_block_still_life);
    RUN_TEST(test_underpopulation);
    RUN_TEST(test_overpopulation);
    RUN_TEST(test_reproduction);

    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
