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
    //malloc array r
    int** gridData = (int**)malloc(ROWS * sizeof(int*));
    //malloc array r(0)... r(4)
        for (int i = 0; i < ROWS; i++){
            gridData[i] = (int*)malloc(COLUMNS * sizeof(int));
        }
        return 0;
    }

//free grid memory after use.
int freeGrid() {
    for (int i = 0; i < ROWS; i++)
        free(gridData[i]);

    free(gridData); // this is causing freeing from heap error because  
    return 0;       // scope error. Global gridData has not been through Malloc?


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

void assignCells(SDL_Surface* surface, int ROWS, int COLUMNS) {
    for (int i = 0; i <= (COLUMNS); i++) {
        for (int j = 0; j <= (ROWS); j++){
            if ((gridData[i][j] = rand() % 2) == 1) {
                SDL_Rect rect = (SDL_Rect){i*15, j*15, cell_width, cell_height};
                SDL_FillRect(surface, &rect, COLOUR_WHITE);
            } else {
                SDL_Rect rect = (SDL_Rect){i*15, j*15, cell_width, cell_height};
                SDL_FillRect(surface, &rect, COLOUR_BLUE);
            }
        }
    }
};

void drawRectangle(SDL_Surface* surface, Uint32 COLOUR_WHITE){
    SDL_Rect rect = (SDL_Rect){15, 15, 15, 15};
    SDL_FillRect(surface, &rect, COLOUR_WHITE);
}

int main(){
    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    allocateGrid(ROWS, COLUMNS);

    //Func. create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);


    //Funcs. draw on surface
    drawGrid(surface, COLUMNS, ROWS, COLOUR_WHITE);
    assignCells(surface, ROWS, COLUMNS);
    //drawRectangle(surface, COLOUR_WHITE);

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
    freeGrid();

    // print grid to check mem freed
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%d ", gridData[i][j]);
        }
        printf("\n");
    }

    printf("Quitting\n");
    return 0;

}

  
