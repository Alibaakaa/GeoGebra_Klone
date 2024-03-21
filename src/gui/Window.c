#include "Window.h"

#include <stdio.h>
#include <stdbool.h>

const Uint32 FRAME_INTERVAL = 20;

Window* Window_create() {
    Window* res = malloc(sizeof(Window));
    if (res == NULL) return NULL;
    memset(res, 0, sizeof(res));

    res->window = SDL_CreateWindow("GeoGebra Klone", 
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   800, 600, 0);
    if (res->window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        Window_destroy(res);
        return NULL;
    }

    res->renderer = SDL_CreateRenderer(res->window, -1, SDL_RENDERER_TARGETTEXTURE);
    if (res->renderer == NULL) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        Window_destroy(res);
        return NULL;
    }

    return res;
}

void Window_destroy(Window* window) {
    if (window == NULL) return;
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    free(window);
}

int Window_eventLoop(Window* window) {
    if (window == NULL) return -1;

    for (bool isOpen = true; isOpen;) {
        Uint32 begin = SDL_GetTicks();
        for (SDL_Event evnt = {}; SDL_PollEvent(&evnt);) {
            if (evnt.type == SDL_QUIT) {
                isOpen = false;
                break;
            }
            // Window_processEvent(window, evnt);
        }

        SDL_RenderClear(window->renderer);
        // all drawing
        SDL_RenderPresent(window->renderer);

        Uint32 timePassed = SDL_GetTicks() - begin;
        if (timePassed < FRAME_INTERVAL) SDL_Delay(FRAME_INTERVAL - timePassed);
    }
}
