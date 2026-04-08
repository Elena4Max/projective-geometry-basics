#include <cmath>
#include <geometry/operations.hpp>

namespace geometry {

bool incidence(const Point2D& p, const Line2D& l) noexcept {
    return std::abs(dot(p, l)) < core::kEps;
}

Point2D normalize(const Point2D& p) noexcept {
    if (std::abs(p.z) < core::kEps) return p;
    return {p.x / p.z, p.y / p.z, 1.0};
}

bool isAtInfinity(const Point2D& p) noexcept { return std::abs(p.z) < core::kEps; }

}  // namespace geometry