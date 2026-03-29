#pragma once
#include "types.h"

Vec3 cross(const Vec3& a, const Vec3& b);
double dot(const Vec3& a, const Vec3& b);

Line2D join(const Point2D& p1, const Point2D& p2);
Point2D meet(const Line2D& l1, const Line2D& l2);

bool incidence(const Point2D& p, const Line2D& l);

Point2D normalize(const Point2D& p);
bool isAtInfinity(const Point2D& p);