#include "Window.h"

#include <stdio.h>
#include <stdbool.h>
#include "Render.h"

const Uint32 FRAME_INTERVAL = 20;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

Window* Window_create() {
    Window* res = malloc(sizeof(Window));
    if (res == NULL) return NULL;
    memset(res, 0, sizeof(res));

    res->window = SDL_CreateWindow("GeoGebra Klone", 
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

void Window_renderSegment(void* data, Point p1, Point p2) {
    Window* window = (Window*) data;
    static const SDL_Color LINE_COLOR = { 255, 255, 255, 255 };
    static const float LINE_WIDTH = 5;

    float distance = hypotf(p1.x - p2.x, p1.y - p2.y);
    float dx = LINE_WIDTH * (p1.y - p2.y) / (2 * distance);
    float dy = LINE_WIDTH * (p2.x - p1.x) / (2 * distance);

    SDL_Vertex vertices[] = {
            { { p1.x + dx, p1.y + dy }, LINE_COLOR },
            { { p1.x - dx, p1.y - dy }, LINE_COLOR },
            { { p2.x - dx, p2.y - dy }, LINE_COLOR },
            { { p2.x + dx, p2.y + dy }, LINE_COLOR }
    };
    int indices[] = { 0, 1, 2, 0, 2, 3 };
    if (SDL_RenderGeometry(window->renderer, NULL, vertices, 4, indices, 6) != 0) {
        fprintf(stderr, "Could not render segment: %s\n", SDL_GetError());
    }
}

void updateSteps(PlotData* plotData) {
    plotData->xStep = plotData->plotRegion.width / (float)plotData->xParts;
    plotData->yStep = plotData->plotRegion.height / (float)plotData->yParts;
}

void Window_rescaleRegion(PlotData* plotData, float scale) {
    Region* region = &plotData->plotRegion;
    float dw = region->width * (scale - 1);
    float dh = region->height * (scale - 1);
    region->width *= scale;
    region->height *= scale;
    region->x -= dw / 2;
    region->y -= dh / 2;
    updateSteps(plotData);
}

void Window_updateAccuracy(PlotData* plotData, float coeff) {
    float newXParts = (float)plotData->xParts * coeff;
    float newYParts = (float)plotData->yParts * coeff;
    plotData->xParts = (size_t)newXParts;
    plotData->yParts = (size_t)newYParts;
    updateSteps(plotData);
}

void Window_processEvent(Window* window, const SDL_Event* evnt, PlotData* plotData) {
    if (evnt->type != SDL_KEYDOWN) return;
    SDL_Keysym keyPressed = evnt->key.keysym;
    switch (keyPressed.sym) {
    case SDLK_LEFT:
        plotData->plotRegion.x -= plotData->plotRegion.width * 0.1f;
        PlotData_evaluateFunction(plotData);
        break;
    case SDLK_RIGHT:
        plotData->plotRegion.x += plotData->plotRegion.width * 0.1f;
        PlotData_evaluateFunction(plotData);
        break;
    case SDLK_UP:
        plotData->plotRegion.y += plotData->plotRegion.height * 0.1f;
        PlotData_evaluateFunction(plotData);
        break;
    case SDLK_DOWN:
        plotData->plotRegion.y -= plotData->plotRegion.height * 0.1f;
        PlotData_evaluateFunction(plotData);
        break;
    case SDLK_EQUALS:
        if (keyPressed.mod & KMOD_SHIFT) {
            Window_updateAccuracy(plotData, 1.1f);
        } else {
            Window_rescaleRegion(plotData, 0.9f);
        }
        PlotData_evaluateFunction(plotData);
        break;
    case SDLK_MINUS:
        if (keyPressed.mod & KMOD_SHIFT) {
            Window_updateAccuracy(plotData, 0.9f);
        } else {
            Window_rescaleRegion(plotData, 1.1f);
        }
        PlotData_evaluateFunction(plotData);
        break;
    }
}

int Window_eventLoop(Window* window, PlotData* plotData) {
    if (window == NULL) return -1;
    Region renderRegion = {
        .x = 0, .y = (float) WINDOW_HEIGHT,
        .width = (float) WINDOW_WIDTH,
        .height = (float) -WINDOW_HEIGHT
    };

    for (bool isOpen = true; isOpen;) {
        Uint32 begin = SDL_GetTicks();
        for (SDL_Event evnt = {}; SDL_PollEvent(&evnt);) {
            if (evnt.type == SDL_QUIT) {
                isOpen = false;
                break;
            }
            Window_processEvent(window, &evnt, plotData);
        }

        SDL_RenderClear(window->renderer);
        RenderPlot(plotData, renderRegion, &Window_renderSegment, window);
        SDL_RenderPresent(window->renderer);

        Uint32 timePassed = SDL_GetTicks() - begin;
        if (timePassed < FRAME_INTERVAL) SDL_Delay(FRAME_INTERVAL - timePassed);
    }
}
