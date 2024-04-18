#pragma once

typedef struct _Point {
    float x;
    float y;
} Point;

typedef struct _Region {
    float x;
    float y;
    float width;
    float height;
} Region;

Point rectangularMap(Region src, Region dst, Point pt);
