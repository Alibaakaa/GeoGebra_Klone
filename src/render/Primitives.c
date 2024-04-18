#include "Primitives.h"

Point rectangularMap(Region src, Region dst, Point pt) {
    Point res = { dst.x + dst.width * (pt.x - src.x) / src.width,
                  dst.y + dst.height * (pt.y - src.y) / src.height };
    return res;
}
