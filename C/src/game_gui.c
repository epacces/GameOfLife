#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define GRID_COLS 120
#define GRID_ROWS 120
#define GRID_SIZE (GRID_COLS * GRID_ROWS)

#define CELL_SIZE 6
#define WINDOW_WIDTH (GRID_COLS * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_ROWS * CELL_SIZE)

#define TARGET_FPS 30

typedef enum { DEAD = 0, ALIVE = 1 } CellState;

// Colors
#define BG_COLOR (Color){15, 15, 25, 255}
#define ALIVE_COLOR (Color){0, 255, 180, 255}
#define GRID_LINE_COLOR (Color){30, 30, 45, 255}

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

void randomize_grid(CellState *grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (rand() % 4 == 0)  // ~25% alive for better patterns
                set_cell(grid, x, y, ALIVE);
        }
    }
}

void draw_grid(const CellState *grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (get_cell(grid, x, y) == ALIVE) {
                // Draw alive cells with a slight glow effect
                int px = x * CELL_SIZE;
                int py = y * CELL_SIZE;

                // Main cell
                DrawRectangle(px, py, CELL_SIZE - 1, CELL_SIZE - 1, ALIVE_COLOR);
            }
        }
    }
}

int main(void) {
    srand(time(NULL));

    CellState grid1[GRID_SIZE], grid2[GRID_SIZE];
    CellState *current_grid = grid1;
    CellState *next_grid = grid2;

    fill_grid(current_grid, DEAD);
    randomize_grid(current_grid);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's Game of Life");
    SetTargetFPS(TARGET_FPS);

    int paused = 0;
    int generation = 0;

    while (!WindowShouldClose()) {
        // Input handling
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
        if (IsKeyPressed(KEY_R)) {
            fill_grid(current_grid, DEAD);
            randomize_grid(current_grid);
            generation = 0;
        }
        if (IsKeyPressed(KEY_C)) {
            fill_grid(current_grid, DEAD);
            generation = 0;
        }

        // Allow drawing cells with mouse
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int mx = GetMouseX() / CELL_SIZE;
            int my = GetMouseY() / CELL_SIZE;
            set_cell(current_grid, mx, my, ALIVE);
        }
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            int mx = GetMouseX() / CELL_SIZE;
            int my = GetMouseY() / CELL_SIZE;
            set_cell(current_grid, mx, my, DEAD);
        }

        // Update simulation
        if (!paused) {
            compute_new_generation(current_grid, next_grid);
            CellState *temp = current_grid;
            current_grid = next_grid;
            next_grid = temp;
            generation++;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BG_COLOR);

        draw_grid(current_grid);

        // Draw UI overlay
        DrawText(TextFormat("Generation: %d", generation), 10, 10, 20, WHITE);
        DrawText(paused ? "PAUSED" : "RUNNING", 10, 35, 20, paused ? YELLOW : GREEN);
        DrawText("SPACE: Pause | R: Randomize | C: Clear | Mouse: Draw", 10, WINDOW_HEIGHT - 25, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
