#include <SDL2/SDL.h>
#include <stdio.h>

#include "Expression.h"
#include "PlotData.h"
#include "Primitives.h"
#include "Window.h"

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
}

Expression* getExpression() {
    char buf[1024];
    printf("Enter function without spaces: ");
    fflush(stdout);
    scanf("%1000s", buf);
    return Expression_create(buf);
}

static const float START_X = -8;
static const float START_Y = -6;
static const float START_WIDTH = 16;
static const float START_HEIGHT = 12;
static const float START_XPARTS = 400;
static const float START_YPARTS = 300;

PlotData* getPlotData() {
    Expression* expr = getExpression();
    if (expr == NULL) return NULL;
    Region plotRegion = {
        .x = START_X, .y = START_Y,
        .width = START_WIDTH,
        .height = START_HEIGHT
    };

    PlotData* res = PlotData_allocate(plotRegion, START_XPARTS, START_YPARTS);
    if (res == NULL) {
        fprintf(stderr, "Could not allocate plot data\n");
        Expression_destroy(expr);
        return NULL;
    }

    res->func = &Expression_evaluate;
    res->funcData = expr;
    res->cleanup = (void (*)(void*))&Expression_destroy;

    PlotData_evaluateFunction(res);

    return res;
}

int main() {
    initSDL();

    PlotData* plotData = getPlotData();
    if (plotData == NULL) return -1;

    Window* window = Window_create();
    int statusCode = Window_eventLoop(window, plotData);

    Window_destroy(window);
    PlotData_destroy(plotData);
    return statusCode;
}
