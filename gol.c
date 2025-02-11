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

    SDL_Rect rect = (SDL_Rect){30, 40, 50, 60};
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