#pragma once

#include "Primitives.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct _PlotData {
    Region plotRegion;
    size_t xParts;
    size_t yParts;
    float xStep;
    float yStep;
    float* functionValues;

    float (*func)(const void*, float, float);
    void* funcData;
    void (*cleanup)(void*);
} PlotData;

PlotData* PlotData_allocate(Region plotRegion, size_t xParts, size_t yParts);
void PlotData_destroy(PlotData* plotData);

Point PlotData_getPoint(const PlotData* plotData, size_t i, size_t j);
float PlotData_getValue(const PlotData* plotData, size_t i, size_t j);
void PlotData_evaluateFunction(PlotData* plotData);
void PlotData_generateCell(const PlotData* plotData, size_t i, size_t j,
                           Point pts[2][2], float values[2][2]);
