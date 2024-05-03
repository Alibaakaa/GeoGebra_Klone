#include "PlotData.h"

PlotData* PlotData_allocate(Region plotRegion, size_t xParts, size_t yParts) {
    PlotData* plotData = (PlotData*) malloc(sizeof(PlotData));
    if (plotData == NULL) return NULL;
    memset(plotData, 0, sizeof(PlotData));

    plotData->plotRegion = plotRegion;
    plotData->xParts = xParts;
    plotData->yParts = yParts;
    plotData->xStep = plotRegion.width / (float) xParts;
    plotData->yStep = plotRegion.height / (float) yParts;

    plotData->functionValues = (float*) calloc((xParts + 1) * (yParts + 1), sizeof(float));
    if (plotData->functionValues == NULL) {
        PlotData_destroy(plotData);
        return NULL;
    }

    return plotData;
}

void PlotData_destroy(PlotData* plotData) {
    if (plotData == NULL) return;
    plotData->cleanup(plotData->funcData);
    free(plotData->functionValues);
    free(plotData);
}

Point PlotData_getPoint(const PlotData* plotData, size_t i, size_t j) {
    Point res = { plotData->plotRegion.x + plotData->xStep * (float) i,
                       plotData->plotRegion.y + plotData->yStep * (float) j };
    return res;
}

float PlotData_getValue(const PlotData* plotData, size_t i, size_t j) {
    return plotData->functionValues[i * (plotData->yParts + 1) + j];
}

void PlotData_evaluateFunction(PlotData* plotData) {
    for (size_t i = 0; i <= plotData->xParts; ++i) {
        for (size_t j = 0; j <= plotData->yParts; ++j) {
            Point pt = PlotData_getPoint(plotData, i, j);
            plotData->functionValues[i * (plotData->yParts + 1) + j] = 
                    plotData->func(plotData->funcData, pt.x, pt.y);
        }
    }
}

void PlotData_generateCell(const PlotData* plotData, size_t i, size_t j,
                           Point pts[2][2], float values[2][2]) {
    for (size_t ii = 0; ii < 2; ++ii) {
        for (size_t jj = 0; jj < 2; ++jj) {
            pts[ii][jj] = PlotData_getPoint(plotData, i + ii, j + jj);
            values[ii][jj] = PlotData_getValue(plotData, i + ii, j + jj);
        }
    }
}
