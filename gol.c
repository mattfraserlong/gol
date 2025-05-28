#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>

// Global constants
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int COLUMNS = 60;
const int ROWS = 40;
const char* WINDOW_TITLE = "Game of Life";
const Uint32 COLOUR_WHITE = 0Xffffffff;
const Uint32 COLOUR_BLACK = 0X00000000;
const Uint32 COLOUR_BLUE = 0X50505050;

// Global variables
bool running = true;

// Allocate memory to grid and assign 0 to each cell with error handling
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
    // Draw vertical lines
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        for (int j = 0; j < WINDOW_WIDTH; j += WINDOW_HEIGHT/COLUMNS){
            SDL_Rect rect = (SDL_Rect){j, i, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
}

// Draw cells onto virtual grid if cell state = 1
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

//Draw single cell onto virtual grid if cell state = 1
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

//create next generation state
int** updateGrid(int** grid, const int ROWS, const int COLUMNS){

    // initialise neighbour count
    int count = 0;

    // Allocate mem and initialise next generation
    int** nextGrid = allocateGrid(ROWS, COLUMNS);

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
                    // Increment neighbour count
                    count = count + grid[xCount][yCount];

                    // Conways Laws here
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
    //main scope variables
    int count = 1;

    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    // allocate memory to 2D array and initialize with 0's
    int** grid = allocateGrid(ROWS, COLUMNS);

    //SDL create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Draw grid onto window surface (virtual) and update 2D array state
    drawGrid(surface, COLUMNS, ROWS, COLOUR_WHITE);

    //draw glider 1 (virtual)
    drawCell(surface, grid, 10, 10);
    drawCell(surface, grid, 10, 9);
    drawCell(surface, grid, 10, 8);
    drawCell(surface, grid, 11, 8);
    drawCell(surface, grid, 12, 9);


    //draw glider 2 (virtual)
    drawCell(surface, grid, 20, 20);
    drawCell(surface, grid, 20, 18);
    drawCell(surface, grid, 20, 16);
    drawCell(surface, grid, 22, 16);
    drawCell(surface, grid, 24, 18);

    //SDL update window surface (virtual to real)
    SDL_UpdateWindowSurface(window);

    //Game loop
    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_KEYDOWN){
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                running = false;
            }
        }


        /*******Game logic goes here*********/

        //Next generation state in 2D array
        int** nextGrid = updateGrid(grid, ROWS, COLUMNS);

        //clear grid for next generation
        clearCells(surface, grid);

        //SDL update window surface
        //SDL_UpdateWindowSurface(window);

        // Draw cells onto virtual grid if cell state = 1
        drawAllCells(surface, nextGrid);

        //print next generation array values in terminal
        print2DArray(nextGrid, ROWS, COLUMNS);

        //SDL update window surface ()
        SDL_UpdateWindowSurface(window);

        // Increment state
        grid = nextGrid;

        count = count + 1;
        printf("Generation No: %d\n", count);

        SDL_Delay(500); // Cap frame rate

    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit(); // Terminate SDL subsystems

    //free memory allocated for grid/nextGrid
    freeArrayMem(grid, ROWS);

    return 0;
}

  