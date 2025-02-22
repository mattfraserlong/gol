//Using SDL: https://wiki.libsdl.org/SDL2/FrontPage

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

SDL_Window* createWindow(const char *WINDOW_TITLE, int WIDTH, int HEIGHT) {
    SDL_Window* window = SDL_CreateWindow (
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );
    return window;
};

void drawGrid(SDL_Surface* surface, int WIDTH, int HEIGHT, Uint32 COLOUR_WHITE) {
     // Draw horizontal grid
     for (int i = 0; i <= WIDTH; i++) {
        for (int j = 0; j <= HEIGHT; j += 15) {
            SDL_Rect rect = (SDL_Rect){i, j, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
    // Draw vertical grid 
    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j += 15) {
            SDL_Rect rect = (SDL_Rect){j, i, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }
};

void drawRectangle(SDL_Surface* surface, Uint32 COLOUR_WHITE) {
    SDL_Rect rect = (SDL_Rect){15, 15, 15, 15};
    SDL_FillRect(surface, &rect, COLOUR_WHITE);
};

int main() {
    // global constants
    const int WIDTH = 900;
    const int HEIGHT = 600;
    const char* WINDOW_TITLE = "Game of Life";
    const Uint32 COLOUR_WHITE = 0Xffffffff;

    // global variables
    bool quit = false;

    //SDL setup
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    //Func. create window
    SDL_Window* window = createWindow(WINDOW_TITLE, WIDTH, HEIGHT);

    //SDL apply surface into window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //Funcs. draw on surface
    drawGrid(surface, WIDTH, HEIGHT, COLOUR_WHITE);
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