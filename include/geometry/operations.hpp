#pragma once
#include <cmath>
#include <core/constants.hpp>
#include <geometry/types.hpp>

namespace geometry {

/// @brief Cross product of two homogeneous vectors.
/// @note Used to compute join (line) and meet (intersection).
constexpr core::Vec3 cross(const core::Vec3& a, const core::Vec3& b) noexcept {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

/// @brief Dot product of two vectors.
constexpr double dot(const core::Vec3& a, const core::Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// @brief Line through two points (join operation).
inline constexpr Line2D join(const Point2D& p1, const Point2D& p2) noexcept {
    return cross(p1, p2);
}

/// @brief Intersection of two lines (meet operation).
inline constexpr Point2D meet(const Line2D& l1, const Line2D& l2) noexcept { return cross(l1, l2); }

/// @brief Checks if point lies on line.
bool incidence(const Point2D& p, const Line2D& l) noexcept;

/// @brief Normalizes homogeneous point (w = 1 if possible).
Point2D normalize(const Point2D& p) noexcept;

/// @brief Returns true if point is at infinity (w ≈ 0).
bool isAtInfinity(const Point2D& p) noexcept;

}  // namespace geometry