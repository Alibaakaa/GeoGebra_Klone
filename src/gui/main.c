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

Region getRenderRegion() {
    Region renderRegion;
    printf("Enter render region (x, y, width, height): ");
    fflush(stdout);
    scanf("%f%f%f%f", &renderRegion.x, &renderRegion.y, &renderRegion.width, &renderRegion.height);
    return renderRegion;
}

struct Parts {
    size_t xParts;
    size_t yParts;
} getRenderAccuracy() {
    struct Parts res;
    printf("Enter render accuracy: ");
    fflush(stdout);
    scanf("%lu%lu", &res.xParts, &res.yParts);
    return res;
}

PlotData* getPlotData() {
    Expression* expr = getExpression();
    if (expr == NULL) return NULL;
    Region plotRegion = getRenderRegion();
    struct Parts parts = getRenderAccuracy();

    PlotData* res = PlotData_allocate(plotRegion, parts.xParts, parts.yParts);
    if (res == NULL) {
        fprintf(stderr, "Could not allocate plot data\n");
        Expression_destroy(expr);
        return NULL;
    }
    PlotData_evaluateFunction(res, &Expression_evaluate, expr);
    Expression_destroy(expr);

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
