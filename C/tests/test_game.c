/*
 * Tests for Game of Life - C implementation
 * Uses a smaller grid (5x5) for easier test verification
 * Compile: make test_game
 * Run: ./test_game
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Use smaller grid for testing - must define before including game_core.h style types */
#define TEST_GRID_COLS 5
#define TEST_GRID_ROWS 5
#define TEST_GRID_SIZE (TEST_GRID_COLS * TEST_GRID_ROWS)

typedef enum { DEAD = 0, ALIVE = 1 } CellState;

/* Core functions with test grid size */
int pos_to_index(int x, int y) {
    x = (x % TEST_GRID_COLS + TEST_GRID_COLS) % TEST_GRID_COLS;
    y = (y % TEST_GRID_ROWS + TEST_GRID_ROWS) % TEST_GRID_ROWS;
    return y * TEST_GRID_COLS + x;
}

void set_cell(CellState *grid, int x, int y, CellState state) {
    grid[pos_to_index(x, y)] = state;
}

CellState get_cell(const CellState *grid, int x, int y) {
    return grid[pos_to_index(x, y)];
}

void fill_grid(CellState *grid, CellState state) {
    for (int i = 0; i < TEST_GRID_SIZE; i++) {
        grid[i] = state;
    }
}

int get_alive_neighbors(const CellState *grid, int x, int y) {
    int n_alive = 0;
    for (int y_off = -1; y_off <= 1; y_off++) {
        for (int x_off = -1; x_off <= 1; x_off++) {
            if (x_off || y_off)
                n_alive += get_cell(grid, x + x_off, y + y_off) == ALIVE ? 1 : 0;
        }
    }
    return n_alive;
}

void compute_new_generation(const CellState *curr_grid, CellState *next_grid) {
    for (int y = 0; y < TEST_GRID_ROWS; y++) {
        for (int x = 0; x < TEST_GRID_COLS; x++) {
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
    assert(pos_to_index(0, 1) == TEST_GRID_COLS);
    assert(pos_to_index(2, 2) == 2 * TEST_GRID_COLS + 2);
}

TEST(test_pos_to_index_wrapping_positive) {
    /* x wraps around */
    assert(pos_to_index(TEST_GRID_COLS, 0) == pos_to_index(0, 0));
    assert(pos_to_index(TEST_GRID_COLS + 1, 0) == pos_to_index(1, 0));
    /* y wraps around */
    assert(pos_to_index(0, TEST_GRID_ROWS) == pos_to_index(0, 0));
    assert(pos_to_index(0, TEST_GRID_ROWS + 1) == pos_to_index(0, 1));
}

TEST(test_pos_to_index_wrapping_negative) {
    /* negative x wraps */
    assert(pos_to_index(-1, 0) == pos_to_index(TEST_GRID_COLS - 1, 0));
    assert(pos_to_index(-2, 0) == pos_to_index(TEST_GRID_COLS - 2, 0));
    /* negative y wraps */
    assert(pos_to_index(0, -1) == pos_to_index(0, TEST_GRID_ROWS - 1));
    assert(pos_to_index(0, -2) == pos_to_index(0, TEST_GRID_ROWS - 2));
}

/* Tests for get_cell and set_cell */
TEST(test_set_and_get_cell) {
    CellState grid[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);

    set_cell(grid, 1, 2, ALIVE);
    assert(get_cell(grid, 1, 2) == ALIVE);
    assert(get_cell(grid, 0, 0) == DEAD);
}

TEST(test_set_cell_with_wrapping) {
    CellState grid[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Set cell using wrapped coordinates */
    set_cell(grid, -1, -1, ALIVE);
    assert(get_cell(grid, TEST_GRID_COLS - 1, TEST_GRID_ROWS - 1) == ALIVE);

    set_cell(grid, TEST_GRID_COLS, TEST_GRID_ROWS, ALIVE);
    assert(get_cell(grid, 0, 0) == ALIVE);
}

/* Tests for get_alive_neighbors */
TEST(test_alive_neighbors_none) {
    CellState grid[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);

    assert(get_alive_neighbors(grid, 2, 2) == 0);
}

TEST(test_alive_neighbors_all_eight) {
    CellState grid[TEST_GRID_SIZE];
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
    CellState grid[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Only the cell itself is alive */
    set_cell(grid, 2, 2, ALIVE);
    assert(get_alive_neighbors(grid, 2, 2) == 0);
}

TEST(test_alive_neighbors_with_wrapping) {
    CellState grid[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);

    /* Cell at corner (0,0), place alive cells at wrapped positions */
    set_cell(grid, TEST_GRID_COLS - 1, TEST_GRID_ROWS - 1, ALIVE);  /* wraps to top-left diagonal */
    set_cell(grid, 1, 0, ALIVE);
    set_cell(grid, 0, 1, ALIVE);

    assert(get_alive_neighbors(grid, 0, 0) == 3);
}

/* Tests for Game of Life rules via compute_new_generation */
TEST(test_blinker_oscillator) {
    CellState grid[TEST_GRID_SIZE], next[TEST_GRID_SIZE];
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
    CellState grid[TEST_GRID_SIZE], next[TEST_GRID_SIZE];
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
    CellState grid[TEST_GRID_SIZE], next[TEST_GRID_SIZE];
    fill_grid(grid, DEAD);
    fill_grid(next, DEAD);

    /* Single cell dies (0 neighbors) */
    set_cell(grid, 2, 2, ALIVE);
    compute_new_generation(grid, next);
    assert(get_cell(next, 2, 2) == DEAD);
}

TEST(test_overpopulation) {
    CellState grid[TEST_GRID_SIZE], next[TEST_GRID_SIZE];
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
    CellState grid[TEST_GRID_SIZE], next[TEST_GRID_SIZE];
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
