#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game_core.h"

#define CELL_SIZE 6
#define WINDOW_WIDTH (GRID_COLS * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_ROWS * CELL_SIZE)

#define TARGET_FPS 30

// Colors
#define BG_COLOR (Color){15, 15, 25, 255}
#define ALIVE_COLOR (Color){0, 255, 180, 255}

void draw_grid(const CellState *grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (get_cell(grid, x, y) == ALIVE) {
                int px = x * CELL_SIZE;
                int py = y * CELL_SIZE;
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
    randomize_grid(current_grid, 4);  // 25% density

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
            randomize_grid(current_grid, 4);
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
