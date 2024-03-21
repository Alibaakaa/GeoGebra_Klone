#include <SDL2/SDL.h>
#include <stdio.h>

#include "Window.h"

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
}

int main() {
    initSDL();

    Window* window = Window_create();
    int statusCode = Window_eventLoop(window);
    Window_destroy(window);

    return statusCode;
}
