#pragma once
#include "../core/mat.h"
#include "../geometry/types.h"

struct Transform2D {
    Mat3 H;

    static Transform2D translation(double tx, double ty);
    static Transform2D rotation(double a);
    static Transform2D scale(double s);

    Point2D apply(const Point2D& p) const;
};