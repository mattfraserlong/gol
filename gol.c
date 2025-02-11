// Using SDL: https://wiki.libsdl.org/SDL2/FrontPage

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main() {
    const char *WINDOW_TITLE = "Game of Life";
    const int WIDTH = 900;
    const int HEIGHT = 600;
    const Uint32 DELAY = 10000;
    const Uint32 COLOUR_WHITE = 0Xffffffff;

    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow (
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    /* Draw horizontal grid*/
    for (int i = 0; i <= WIDTH; i++) {
        for (int j = 0; j <= HEIGHT; j += 15) {
            SDL_Rect rect = (SDL_Rect){i, j, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }

    /* Draw vertical grid*/
    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j += 15) {
            SDL_Rect rect = (SDL_Rect){j, i, 1, 1};
            SDL_FillRect(surface, &rect, COLOUR_WHITE);
        }
    }

    /*Add a square into the grid*/
    SDL_Rect rect = (SDL_Rect){15, 15, 15, 15};
    SDL_FillRect(surface, &rect, COLOUR_WHITE);



    SDL_UpdateWindowSurface(window);

    /* loop to make window appear and quit on key press*/
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