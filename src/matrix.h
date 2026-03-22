#pragma once
#include "geometry.h"

struct Mat3 {
    double m[3][3];
};

Point applyHomography(const Mat3& H, const Point& p);
Line transformLine(const Mat3& H, const Line& l);
