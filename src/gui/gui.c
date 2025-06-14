#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <math.h>
#include "../matrix/matrix.h"

#define CELL_SIZE 20
#define GRID_SIZE 28
#define EPSILON 0.001f

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

App app;
float pixelGrid[GRID_SIZE][GRID_SIZE];

// INIT SDL
int initSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    app.window = SDL_CreateWindow(
        "SDL Pixel Editor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GRID_SIZE * CELL_SIZE,
        GRID_SIZE * CELL_SIZE,
        SDL_WINDOW_SHOWN
    );
    if (app.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (app.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
    return 1;
}

void closeSDL(void) {
    SDL_DestroyRenderer(app.renderer);
    app.renderer = NULL;

    SDL_DestroyWindow(app.window);
    app.window = NULL;

    SDL_Quit();
}

// DRAW
void renderGrid(void) {
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);

    for (int i=0; i < GRID_SIZE; i++) {
        for (int j=0; j < GRID_SIZE; j++) {
            SDL_Rect rect = {
                i * CELL_SIZE,
                j * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
            };

            Uint8 alpha = (Uint8)(fmaxf(0.0f, fminf(1.0f, pixelGrid[i][j])) * 255.0f);
            SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, alpha); 

            SDL_RenderFillRect(app.renderer, &rect);
            // SDL_SetRenderDrawColor(app.renderer, 0x08, 0x08, 0x08, 0xFF);
            // SDL_RenderDrawRect(app.renderer, &rect);
        }
    }
    SDL_RenderPresent(app.renderer);
}

// HANDLER
int isDrawing = 0;

void initializeGridData() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            pixelGrid[i][j] = 0;
        }
    }
}

void applyBrush(int centerRow, int centerCol, float valueToSet) {
    struct { int dr, dc; float value; } brush_pattern[] = {
        {-1, -1, 0.3f}, { -1, 0, 0.7f}, {-1, 1, 0.3f},
        { 0, -1, 0.7f}, {  0, 0, 1.0f}, { 0, 1, 0.7f},
        { 1, -1, 0.3f}, {  1, 0, 0.7f}, { 1, 1, 0.3f}
    };
    int pattern_size = sizeof(brush_pattern) / sizeof(brush_pattern[0]);

    for (int i = 0; i < pattern_size; i++) {
        int targetRow = centerRow + brush_pattern[i].dr;
        int targetCol = centerCol + brush_pattern[i].dc;

        // Apply boundary checks
        if (targetRow >= 0 && targetRow < GRID_SIZE &&
            targetCol >= 0 && targetCol < GRID_SIZE) {
            if (fabsf(valueToSet - 0.0f) < EPSILON) {
                pixelGrid[targetRow][targetCol] = 0.0f;
            } else {
                float current_val = pixelGrid[targetRow][targetCol];
                float brush_val = brush_pattern[i].value;

                pixelGrid[targetRow][targetCol] = fminf(current_val + brush_val, 1.0f);
            }
        }
    }
}

int handleEvents(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return 0;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                isDrawing = 1;
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                int clickedRow = mouseX / CELL_SIZE;
                int clickedCol = mouseY / CELL_SIZE;

                applyBrush(clickedRow, clickedCol, 1.0f);
                renderGrid();
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                isDrawing = 0;
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            if (isDrawing) {
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;

                int currentRow = mouseX / CELL_SIZE;
                int currentCol = mouseY / CELL_SIZE;

                applyBrush(currentRow, currentCol, 1.0f);
                renderGrid();
            }
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_c:
                    initializeGridData();
                    renderGrid();
                    break;
                case SDLK_RETURN:
                    return 0;
                case SDLK_ESCAPE:
                    return 0;
            }
        }
    }
    return 1;
}

// MAIN
Matrix *executeGui(void) {
    memset(&app, 0, sizeof(App));
    initSDL();
    initializeGridData();

    int quit = 0;
    renderGrid();

    while (!quit) {
        quit = !handleEvents();
    }
    closeSDL();

    // make new matrix
    Matrix *mat = matrix_create(28, 28);
    for (int i=0; i < mat->rows; i++) {
        for (int j=0; j < mat->cols; j++) {
            mat->entries[i][j] = pixelGrid[j][i];
        }
    }
    return mat;
}
