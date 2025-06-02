#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// Global constants
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const int COLUMNS = 30;
const int ROWS = 20;
const char* WINDOW_TITLE = "Game of Life";
const Uint32 COLOUR_WHITE = 0Xffffffff;
const Uint32 COLOUR_BLACK = 0X00000000;

// Global variables
bool running = true;
bool paused = false;

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
void drawGrid(SDL_Surface* surface, const int num_rows, const int num_cols, Uint32 color, SDL_Window* window) {
    int cell_width = WINDOW_WIDTH / num_cols;
    int cell_height = WINDOW_HEIGHT / num_rows;
    // Draw horizontal lines
    for (int i = 0; i <= num_rows; i++) {
        SDL_Rect line = {0, i * cell_height, WINDOW_WIDTH, 1};
        if (i * cell_height == WINDOW_HEIGHT) { // Prevent drawing outside bounds if line is exactly at edge
             line.y -=1;
        }
        SDL_FillRect(surface, &line, color);
    }
    // Draw vertical lines
    for (int j = 0; j <= num_cols; j++) {
        SDL_Rect line = {j * cell_width, 0, 1, WINDOW_HEIGHT};
         if (j * cell_width == WINDOW_WIDTH) { // Prevent drawing outside bounds
             line.x -=1;
        }
        SDL_FillRect(surface, &line, color);
    }
}

// Draw cells onto virtual grid if cell state = 1
void drawAllCells(SDL_Surface* surface, int** grid, SDL_Window* window){
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
    //SDL update window surface
    SDL_UpdateWindowSurface(window);
}

//Draw single cell onto virtual grid if cell state = 1
void drawCell(SDL_Surface* surface, int** grid, int cell_x, int cell_y, SDL_Window* window){
    int pos_x = (cell_x) * (WINDOW_WIDTH/COLUMNS);
    int pos_y = (cell_y) * (WINDOW_HEIGHT/ROWS);
    grid[cell_y][cell_x] = 1;
    SDL_Rect cell_rect = (SDL_Rect){pos_x + 2, pos_y + 2, 17, 17};
    SDL_FillRect(surface, &cell_rect, COLOUR_WHITE);
    //SDL update window surface
    SDL_UpdateWindowSurface(window);
}

//Overdraw all cells with black cells
void clearCells(SDL_Surface* surface, SDL_Window* window){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++){
            int pos_x = j * (WINDOW_WIDTH/COLUMNS);
            int pos_y = i * (WINDOW_HEIGHT/ROWS);
            SDL_Rect cell_rect = (SDL_Rect){pos_x + 2, pos_y + 2, 17, 17};
            SDL_FillRect(surface, &cell_rect, COLOUR_BLACK);
        }
    }
    //SDL update window surface
    SDL_UpdateWindowSurface(window);
}

//print array state to terminal
void print2DArray (int** grid, const int ROWS, const int COLUMNS){
    for (int i = 0; i < ROWS; i++){
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
                    if (xCount == -1) {xCount = ROWS - 1;}
                    if (xCount == ROWS) {xCount = 0;}
                    if (yCount == -1) {yCount = COLUMNS -1;}
                    if (yCount == COLUMNS) {yCount = 0;}
                    //printf("%d", nextGrid[xCount][yCount]);
                    // Increment neighbour count
                    count = count + grid[xCount][yCount];
                }
            }
            /***********Conway's Game of Life rules**************/
            if (grid[i][j] == 1) { //If current cell alive...
                // Rule 1: Any live cell with fewer than two live neighbours dies (underpopulation).
                if (count < 2) {
                    nextGrid[i][j] = 0;
                }
                // Rule 2: Any live cell with two or three live neighbours lives on to the next generation.
                else if (count == 2 || count == 3) {
                    nextGrid[i][j] = 1;
                }
                // Rule 3: Any live cell with more than three live neighbours dies (overpopulation).
                else if (count > 3) {
                    nextGrid[i][j] = 0;
                }
            } else { //If current cell dead...
                // Rule 4: Any dead cell with exactly three live neighbours becomes a live cell (reproduction).
                if (count == 3) {
                    nextGrid[i][j] = 1;
                } else {
                    nextGrid[i][j] = 0;
                } 
            }
            // Reset neighbour count
            count = 0;
        }
    }
    return nextGrid;
}

void freeArrayMem(int** grid, int ROWS){
    for (int i = 0; i < ROWS; i++){
        free(grid[i]);
    }
    free(grid);
}

int** init (SDL_Window* window, SDL_Surface* surface) {
    int** grid = allocateGrid(ROWS, COLUMNS);
    clearCells(surface, window);
    SDL_UpdateWindowSurface(window);
    drawAllCells(surface, grid, window);
    return grid;
}

void drawGliders(SDL_Window* window, SDL_Surface* surface, int**grid) {
    //draw glider 1 (virtual)
    drawCell(surface, grid, 10, 10, window);
    drawCell(surface, grid, 11, 11, window);
    drawCell(surface, grid, 12, 9, window);
    drawCell(surface, grid, 12, 10, window);
    drawCell(surface, grid, 12, 11, window);
    /*draw glider 2 (virtual)
    drawCell(surface, grid, 20, 20, window);
    drawCell(surface, grid, 21, 21, window);
    drawCell(surface, grid, 22, 19, window);
    drawCell(surface, grid, 22, 20, window);
    drawCell(surface, grid, 22, 21, window);
    */
    //SDL update window surface (virtual to real)
    SDL_UpdateWindowSurface(window);
}

int main(){
    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    //SDL create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Draw grid onto window surface (virtual) and update 2D array state
    drawGrid(surface, ROWS, COLUMNS, COLOUR_WHITE, window);
    //initialise game
    int** grid = init(window, surface);
    //draw gliders
    drawGliders(window, surface, grid);

    //Game loop
    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE){
                    paused = !paused;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN){
                Sint32 mouseX = event.button.x;
                Sint32 mouseY = event.button.y;
                //SDL_GetMouseState(&mouseX, &mouseY); // Gets mouse state relative to the window
                printf("Mouse Coordinates: X = %d, Y = %d\n", mouseX, mouseY);
            }
            if (!paused) {
                /***********Game logic goes here*********/
                /****************************************/

                //Next generation state in 2D array
                int** nextGrid = updateGrid(grid, ROWS, COLUMNS);

                //clear grid for next generation
                clearCells(surface, window);

                // Draw cells onto virtual grid if cell state = 1
                drawAllCells(surface, nextGrid, window);

                // Increment state and deallocate memory
                int** oldGrid = grid;
                grid = nextGrid; // grid now points to the new generation data
                if (oldGrid != NULL) { // Ensure oldGrid was actually allocated
                    freeArrayMem(oldGrid, ROWS); // Free the old generation's data
                }
                SDL_Delay(100); // Cap frame rate
            }
        }
    }
    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit(); // Terminate SDL subsystems

    return 0;
}

  