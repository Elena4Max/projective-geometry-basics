#pragma once
#include <cmath>

struct Vec3 {
    double x, y, z;
};

using Point = Vec3; // (x, y, w)
using Line  = Vec3; // (a, b, c)

Vec3 cross(const Vec3& a, const Vec3& b);
double dot(const Vec3& a, const Vec3& b);

Line join(const Point& p1, const Point& p2);
Point meet(const Line& l1, const Line& l2);
bool incidence(const Point& p, const Line& l);

Point normalize(const Point& p);
bool isAtInfinity(const Point& p);

void printPoint(const Point& p);
void printLine(const Line& l);
