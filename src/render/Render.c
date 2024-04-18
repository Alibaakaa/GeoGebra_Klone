#include "Render.h"

static void (*g_renderCall)(void*, Point, Point) = NULL;
static void* g_renderData = NULL;
static Region g_plotRegion = {};
static Region g_renderRegion = {};

void renderSegment(Point p1, Point p2) {
    Point q1 = rectangularMap(g_plotRegion, g_renderRegion, p1);
    Point q2 = rectangularMap(g_plotRegion, g_renderRegion, p2);
    g_renderCall(g_renderData, q1, q2);
}

float findRoot(float x1, float x2, float val1, float val2) {
    float t = val1 / (val1 - val2);
    return x1 + t * (x2 - x1);
}

void renderAngle(Point pts[2][2], float values[2][2], int i, int j) {
    Point p1 = { findRoot(pts[i][j].x, pts[i ^ 1][j].x, values[i][j], values[i ^ 1][j]), 
                 pts[i][j].y };
    Point p2 = { pts[i][j].x,
                 findRoot(pts[i][j].y, pts[i][j ^ 1].y, values[i][j], values[i][j ^ 1]) };
    renderSegment(p1, p2);
}

void renderPositiveAngle(Point pts[2][2], float values[2][2]) {
    int iPos = -1, jPos = -1;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (values[i][j] > 0) {
                iPos = i;
                jPos = j;
                break;
            }
        }
    }
    renderAngle(pts, values, iPos, jPos);
}

void renderNegativeAngle(Point pts[2][2], float values[2][2]) {
    int iNeg = -1, jNeg = -1;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (values[i][j] <= 0) {
                iNeg = i;
                jNeg = j;
                break;
            }
        }
    }
    renderAngle(pts, values, iNeg, jNeg);
}

void renderVertical(Point pts[2][2], float values[2][2]) {
    Point p1 = { findRoot(pts[0][0].x, pts[1][0].x, values[0][0], values[1][0]), pts[0][0].y };
    Point p2 = { findRoot(pts[0][1].x, pts[1][1].x, values[0][1], values[1][1]), pts[0][1].y };
    renderSegment(p1, p2);
}

void renderHorizontal(Point pts[2][2], float values[2][2]) {
    Point p1 = { pts[0][0].x, findRoot(pts[0][0].y, pts[0][1].y, values[0][0], values[0][1]) };
    Point p2 = { pts[1][0].x, findRoot(pts[1][0].y, pts[1][1].y, values[1][0], values[1][1]) };
    renderSegment(p1, p2);
}

void RenderPlot(const PlotData* plotData, Region renderRegion,
                void (*renderFunc)(void*, Point, Point), void* data) {
    g_renderCall = renderFunc;
    g_renderData = data;
    g_plotRegion = plotData->plotRegion;
    g_renderRegion = renderRegion;

    for (size_t i = 0; i < plotData->xParts; ++i) {
        for (size_t j = 0; j < plotData->yParts; ++j) {
            float values[2][2];
            Point pts[2][2];
            PlotData_generateCell(plotData, i, j, pts, values);

            int cntPositive = (values[0][0] > 0) + (values[0][1] > 0) +
                              (values[1][0] > 0) + (values[1][1] > 0);
            if (cntPositive == 1) renderPositiveAngle(pts, values);
            if (cntPositive == 3) renderNegativeAngle(pts, values);
            if (cntPositive == 2) {
                if (((values[0][0] > 0) ^ (values[0][1] > 0)) &&
                    ((values[0][0] > 0) ^ (values[1][0] > 0))) {
                    renderAngle(pts, values, 0, 0);
                    renderAngle(pts, values, 1, 1);
                } else if ((values[0][0] > 0) ^ (values[0][1] > 0)) {
                    renderHorizontal(pts, values);
                } else {
                    renderVertical(pts, values);
                }
            }
        }
    }
}
