#include "geometry.h"
#include <iostream>

Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

double dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Line join(const Point& p1, const Point& p2) {
    return cross(p1, p2);
}

Point meet(const Line& l1, const Line& l2) {
    return cross(l1, l2);
}

bool incidence(const Point& p, const Line& l) {
    return std::abs(dot(p, l)) < 1e-6;
}

Point normalize(const Point& p) {
    if (std::abs(p.z) < 1e-9) {
        return p;
    }

    return { p.x / p.z, p.y / p.z, 1.0 };
}

bool isAtInfinity(const Point& p) {
    return std::abs(p.z) < 1e-9;
}

void printPoint(const Point& p) {
    std::cout << "Point: (" << p.x << ", " << p.y << ", " << p.z << ")\n";
}

void printLine(const Line& l) {
    std::cout << "Line: " << l.x << "x + " << l.y << "y + " << l.z << " = 0\n";
}
