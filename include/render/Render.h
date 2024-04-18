#pragma once

#include "Primitives.h"
#include "PlotData.h"

void RenderPlot(const PlotData* plotData, Region renderRegion,
                void (*renderFunc)(void*, Point, Point), void* data);
