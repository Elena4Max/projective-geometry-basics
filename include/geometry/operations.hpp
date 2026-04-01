#pragma once
#include <geometry/types.hpp>
#include <core/constants.hpp>
#include <cmath>

namespace geometry {

    constexpr core::Vec3 cross(const core::Vec3& a, const core::Vec3& b) noexcept {
        return {
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        };
    }

    constexpr double dot(const core::Vec3& a, const core::Vec3& b) noexcept {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    inline constexpr Line2D join(const Point2D& p1, const Point2D& p2) noexcept {
        return cross(p1, p2);
    }

    inline constexpr Point2D meet(const Line2D& l1, const Line2D& l2) noexcept {
        return cross(l1, l2);
    }

    bool incidence(const Point2D& p, const Line2D& l) noexcept;
    Point2D normalize(const Point2D& p) noexcept;
    bool isAtInfinity(const Point2D& p) noexcept;

} // namespace geometry