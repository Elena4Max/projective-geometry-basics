#pragma once
#include "../core/mat.h"
#include "../geometry/types.h"

struct Homography {
    Mat3 H;

    Point2D transformPoint(const Point2D& p) const;
    Line2D transformLine(const Line2D& l) const;
};