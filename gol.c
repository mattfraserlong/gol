//Using SDL: https://wiki.libsdl.org/SDL2/FrontPage

/* Conways laws */
/*
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// global constants
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int COLUMNS = 60;
const int ROWS = 40;
const char* WINDOW_TITLE = "Game of Life";
const Uint32 COLOUR_WHITE = 0Xffffffff;
const Uint32 COLOUR_BLACK = 0X00000000;
const Uint32 COLOUR_BLUE = 0X50505050;

// global variables
bool quit = false;
int gridData[ROWS][COLUMNS];
int cell_width = WINDOW_WIDTH/COLUMNS-2;
int cell_height = WINDOW_HEIGHT/ROWS-2;

//Allocate memory to grid
int allocateGrid(const int ROWS, const int COLUMNS) {
    int** gridData = (int**)malloc(ROWS * sizeof(int*));
        for (int i = 0; i < ROWS; i++){
            gridData[i] = (int*)malloc(COLUMNS * sizeof(int));
        }
        return 0;
    }

//SDL create window
SDL_Window* createWindow(const char *WINDOW_TITLE, int WINDOW_WIDTH, int WINDOW_HEIGHT){
    SDL_Window* window = SDL_CreateWindow (
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );
    return window;
};

//Draw grid matrix
void drawGrid(SDL_Surface* surface, int ROWS, int COLUMNS, Uint32 COLOUR_WHITE){
     // Draw horizontal grid
     for (int i = 0; i <= WINDOW_WIDTH; i++) {
        for (int j = 0; j <= WINDOW_HEIGHT; j += WINDOW_WIDTH/ROWS) {
            SDL_Rect rect = (SDL_Rect){i, j, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
    /* Draw vertical grid */
    for (int i = 0; i <= WINDOW_HEIGHT; i++) {
        for (int j = 0; j <= WINDOW_WIDTH; j += WINDOW_HEIGHT/COLUMNS) {
            SDL_Rect rect = (SDL_Rect){j, i, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
};

void drawCell(SDL_Surface* surface, int cell_x, int cell_y){
    int pos_x = (cell_x - 1) * (WINDOW_WIDTH/COLUMNS);
    int pos_y = (cell_y - 1) * (WINDOW_HEIGHT/ROWS);
    SDL_Rect cell_rect = (SDL_Rect){pos_x, pos_y, 15, 15};
    SDL_FillRect(surface, &cell_rect, COLOUR_WHITE);
}

int main(){
    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    // allocate memory to grid array
    allocateGrid(ROWS, COLUMNS);

    //SDL create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Draw on surface
    drawGrid(surface, COLUMNS, ROWS, COLOUR_WHITE);

    //draw one grid square
    drawCell(surface, 10, 10);

    //SDL draw virtual rectangles onto surface
    SDL_UpdateWindowSurface(window);

    //Loop to make window quit on key press
    while (!quit){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_KEYDOWN){
                quit = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }

    printf("Quitting\n");
    return 0;
}

  