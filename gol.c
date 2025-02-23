//Using SDL: https://wiki.libsdl.org/SDL2/FrontPage

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

// global variables
bool quit = false;
int** gridData;

//PROBLEM HERE... WHAT DH TO SEE HOW HE HANDLED INTIALIZING
//2D ARRAY WITH POINTERS AVOIDING SEGMENTATION FAULT.

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

void assignCells(int ROWS, int COLUMNS) {
    for (int i = 0; i <= ROWS; i++) {
        for (int j = 0; j <= COLUMNS; j++){
            gridData[i][j] = rand() % 2;
            printf("Row = %d, Column = %d, data = %d\n", i, j, gridData[i][j]);
        }
    }
};

void drawRectangle(SDL_Surface* surface, Uint32 COLOUR_WHITE){
    SDL_Rect rect = (SDL_Rect){15, 15, 15, 15};
    SDL_FillRect(surface, &rect, COLOUR_WHITE);
};

int main(){
    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    //Func. create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Funcs. draw on surface
    drawGrid(surface, COLUMNS, ROWS, COLOUR_WHITE);
    assignCells(ROWS, COLUMNS);
    drawRectangle(surface, COLOUR_WHITE);

    //SDL draw virtual rects onto surface
    SDL_UpdateWindowSurface(window);

    //Loop to make window appear and quit on key press
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