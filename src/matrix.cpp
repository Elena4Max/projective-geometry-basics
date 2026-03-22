#include "matrix.h"

// H * p
Point applyHomography(const Mat3& H, const Point& p) {
    Point r;
    r.x = H.m[0][0]*p.x + H.m[0][1]*p.y + H.m[0][2]*p.z;
    r.y = H.m[1][0]*p.x + H.m[1][1]*p.y + H.m[1][2]*p.z;
    r.z = H.m[2][0]*p.x + H.m[2][1]*p.y + H.m[2][2]*p.z;
    return r;
}

// H^{-T} * l
Line transformLine(const Mat3& H, const Line& l) {
    
    return l;
}
