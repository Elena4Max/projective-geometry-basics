#include "../include/projective/homography.h"

Point2D Homography::transformPoint(const Point2D& p) const {
    return H * p;
}

Line2D Homography::transformLine(const Line2D& l) const {
    Mat3 invT = H.inverse().transpose();
    return invT * l;
}