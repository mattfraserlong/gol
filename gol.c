#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>

// global constants
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const int COLUMNS = 30;
const int ROWS = 20;
const char* WINDOW_TITLE = "Game of Life";
const Uint32 COLOUR_WHITE = 0Xffffffff;
const Uint32 COLOUR_BLACK = 0X00000000;
const Uint32 COLOUR_BLUE = 0X50505050;

// global variables
bool quit = false;

//Allocate memory to grid and assign 0 to each cell with error handling
int** allocateGrid(const int ROWS, const int COLUMNS) {
    int** gridData = NULL;
    // Allocate memory for the row pointers
    gridData = (int**)malloc(ROWS * sizeof(int*));
    if (gridData == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for row pointers.\n");
        return NULL; // Indicate failure
    }
    // Allocate memory for each row
    for (int i = 0; i < ROWS; i++) {
        gridData[i] = (int*)malloc(COLUMNS * sizeof(int));
        if (gridData[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for row %d.\n", i);
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(gridData[j]);
            }
            free(gridData);
            return NULL; // Indicate failure
        }
    }
    // Initialize the grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            gridData[i][j] = 0;
        }
    }
    return gridData; // Return the allocated grid
}

//SDL create window
SDL_Window* createWindow(const char* WINDOW_TITLE, int WINDOW_WIDTH, int WINDOW_HEIGHT){
    SDL_Window* window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );
    return window;
};

//Draw grid line
void drawGrid(SDL_Surface* surface, const int ROWS, const int COLUMNS, Uint32 COLOUR_WHITE){
     // Draw horizontal lines
     for (int i = 0; i < WINDOW_WIDTH; i++){
        for (int j = 0; j < WINDOW_HEIGHT; j+= WINDOW_WIDTH/ROWS){
            SDL_Rect rect = (SDL_Rect){i, j, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
    /* Draw vertical lines */
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j += WINDOW_HEIGHT/COLUMNS){
            SDL_Rect rect = (SDL_Rect){j, i, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
}

// Draw all squares
void drawAllCells(SDL_Surface* surface, int** grid){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++){
            int pos_x = j * (WINDOW_WIDTH/COLUMNS);
            int pos_y = i * (WINDOW_HEIGHT/ROWS);
            if (grid[i][j] == 1){
                SDL_Rect cell_rect = (SDL_Rect){pos_x + 2, pos_y + 2, 17, 17};
                SDL_FillRect(surface, &cell_rect, COLOUR_WHITE);
            }
        }
    }
}

//Draw single cell
void drawCell(SDL_Surface* surface, int** grid, int cell_x, int cell_y){
    int pos_x = (cell_x) * (WINDOW_WIDTH/COLUMNS);
    int pos_y = (cell_y) * (WINDOW_HEIGHT/ROWS);
    grid[cell_y][cell_x] = 1;
    SDL_Rect cell_rect = (SDL_Rect){pos_x + 2, pos_y + 2, 17, 17};
    SDL_FillRect(surface, &cell_rect, COLOUR_WHITE);
}

//Overdraw all cells with black cells
void clearCells(SDL_Surface* surface, int** grid){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++){
            int pos_x = i * (WINDOW_WIDTH/COLUMNS);
            int pos_y = j * (WINDOW_HEIGHT/ROWS);
            SDL_Rect cell_rect = (SDL_Rect){pos_x + 2, pos_y + 2, 17, 17};
            SDL_FillRect(surface, &cell_rect, COLOUR_BLACK);
        }
    }
}

//print array state to terminal
void print2DArray (int** grid, const int ROWS, const int COLUMNS){
    for (int i = 0; i < ROWS-1; i++){
        for (int j = 0; j < COLUMNS; j++){
            printf("%d", grid[i][j]);
        }
        printf(" ROW END %d\n", i);
    }
    printf("ARRAY END\n");
}

//pause one second
void pause_one_second() {
    sleep(3);
}

//create next generation state
int** nextGeneration(int** grid, const int ROWS, const int COLUMNS){

    int** nextGrid = allocateGrid(ROWS, COLUMNS);

    int count = 0;

    //calculate neighbour score with wrap around
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            for (int x = -1; x <= 1; x++){
                for (int y = -1; y <= 1; y++){
                    int xCount = i + x;
                    int yCount = j + y;
                        if (x == 0 && y == 0) {
                            continue;
                        }
                    if (xCount == -1) {xCount = ROWS;}
                    if (xCount == ROWS) {xCount = 0;}
                    if (yCount == -1) {yCount = COLUMNS;}
                    if (yCount == COLUMNS) {yCount = 0;}

                    //printf("%d", nextGrid[xCount][yCount]);
                    count = count + grid[xCount][yCount];
                    //laws here

                    if (grid[i][j] == 1) {
                        // Rule 1: Any live cell with fewer than two live neighbours dies (underpopulation).
                        if (count < 2) {
                            nextGrid[i][j] = 0;
                            //printf("law 1 enacted nextGrid cell has changed to: %d \n", nextGrid[i][j]);
                        }
                        // Rule 2: Any live cell with two or three live neighbours lives on to the next generation.
                        else if (count == 2 || count == 3) {
                            nextGrid[i][j] = 1;
                            //printf("law 2 enacted nextGrid cell has changed to: %d \n", nextGrid[i][j]);
                        }
                        // Rule 3: Any live cell with more than three live neighbours dies (overpopulation).
                        else if (count > 3) {
                            nextGrid[i][j] = 0;
                            //printf("law 3 enacted nextGrid cell has changed to: %d \n", nextGrid[i][j]);
                        }
                        } else { // current_grid[r][c] == DEAD
                        // Rule 4: Any dead cell with exactly three live neighbours becomes a live cell (reproduction).
                        if (count == 3) {
                            nextGrid[i][j] = 1;
                        } else {
                            nextGrid[i][j] = 0; // Stays dead
                        }
                        //printf("law 4 enacted nextGrid cell has changed to: %d \n", nextGrid[i][j]);
                    }
                }
                //printf("\n");
            }
            //printf("Neighbour count for cell %d, %d = %d \n", i, j, count);
            count = 0;
        }
    }
    //printf("Count of neighbour cells is: %d \n", count);
    
    return nextGrid;
}

void freeArrayMem(int** grid, int ROWS){
    for (int i = 0; i < ROWS; i++){
        free(grid[i]);
    }
    free(grid);
}

int main(){
    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    // allocate memory to grid arrays and initialize with 0's
    int** grid = allocateGrid(ROWS, COLUMNS);

    //SDL create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Draw on surface
    drawGrid(surface, COLUMNS, ROWS, COLOUR_WHITE);

    //draw glider
    drawCell(surface, grid, 10, 10);
    drawCell(surface, grid, 10, 9);
    drawCell(surface, grid, 10, 8);
    drawCell(surface, grid, 11, 8);
    drawCell(surface, grid, 12, 9);

    //print array values in terminal
    print2DArray(grid, ROWS, COLUMNS);

    //SDL update window surface
    SDL_UpdateWindowSurface(window);

    //Create second generation
    int** secondGrid = nextGeneration(grid, ROWS, COLUMNS);

    //clear grid for next generation
    clearCells(surface, grid);

    //SDL update window surface
    SDL_UpdateWindowSurface(window);

    //draw second generation onto grid
    drawAllCells(surface, secondGrid);

    //slow execution for debug.
    //pause_one_second();

    //print second generation array values in terminal
    print2DArray(secondGrid, ROWS, COLUMNS);

    //SDL update window surface
    SDL_UpdateWindowSurface(window);

    //Don't forget to terminate SDL and use better SDL draw system.

    //Quit loop
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
    freeArrayMem(grid, ROWS);
    //freeArrayMem(grid, ROWS);
    return 0;
}

  