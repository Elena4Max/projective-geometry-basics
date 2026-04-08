#pragma once
#include <core/mat.hpp>
#include <geometry/types.hpp>

namespace transform {

/// @brief 2D transformation represented as a 3x3 matrix.
struct Transform2D {
    core::Mat3 H;

    /// @brief Creates translation transform.
    static Transform2D translation(double tx, double ty) noexcept;

    /// @brief Creates rotation transform (radians).
    static Transform2D rotation(double a) noexcept;

    /// @brief Creates uniform scaling transform.
    static Transform2D scale(double s) noexcept;

    /// @brief Applies transform to a point.
    geometry::Point2D apply(const geometry::Point2D& p) const noexcept;
};

}  // namespace transform