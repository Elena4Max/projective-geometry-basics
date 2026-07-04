#include <camera/projection.hpp>

namespace camera {

geometry::Point2D projectPoint(const core::Vec3& X, const CameraIntrinsics& K) noexcept {
    auto pn = geometry::normalize(geometry::Point2D{X.x, X.y, X.z});

    const double u = K.fx * pn.x + K.cx;
    const double v = K.fy * pn.y + K.cy;

    return {u, v, 1.0};
}

geometry::Point2D projectPoint(const core::Vec3& X, const CameraIntrinsics& K,
                               const CameraExtrinsics& E) noexcept {
    auto Xc = E.R * X;

    Xc.x += E.t.x;
    Xc.y += E.t.y;
    Xc.z += E.t.z;

    return projectPoint(Xc, K);
}

double reprojectionError(const geometry::Point2D& observed,
                         const geometry::Point2D& projected) noexcept {
    return geometry::euclideanDistance(observed, projected);
}

}  // namespace camera
