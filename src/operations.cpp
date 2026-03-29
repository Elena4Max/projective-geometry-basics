#include "../include/geometry/operations.h"
#include <cmath>

static constexpr double EPS = 1e-9;

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

Line2D join(const Point2D& p1, const Point2D& p2) {
    return cross(p1,p2);
}

Point2D meet(const Line2D& l1, const Line2D& l2) {
    return cross(l1,l2);
}

bool incidence(const Point2D& p, const Line2D& l) {
    return std::abs(dot(p,l)) < EPS;
}

Point2D normalize(const Point2D& p) {
    if(std::abs(p.z) < EPS) return p;
    return {p.x/p.z, p.y/p.z, 1.0};
}

bool isAtInfinity(const Point2D& p) {
    return std::abs(p.z) < EPS;
}