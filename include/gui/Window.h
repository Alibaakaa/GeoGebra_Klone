#pragma once

#include <SDL2/SDL.h>

typedef struct _Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
} Window;

Window* Window_create();
void Window_destroy(Window* window);

int Window_eventLoop(Window* window);
