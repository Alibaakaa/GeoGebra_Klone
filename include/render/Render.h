#pragma once

#include "Primitives.h"
#include "PlotData.h"

void RenderGrid(Region plotRegion, Region renderRegion,
                void (*renderFunc)(void*, Point, Point), void* data);

void RenderPlot(const PlotData* plotData, Region renderRegion,
                void (*renderFunc)(void*, Point, Point), void* data);
