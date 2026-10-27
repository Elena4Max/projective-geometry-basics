#pragma once
#include <core/mat.hpp>
#include <geometry/types.hpp>

namespace projective {
/// @brief Homography represented by a 3x3 matrix.
struct Homography {
    core::Mat3 H;

    /// @brief Applies homography to a line.
    /// @note Uses dual transform: l' = H^{-T} l
    geometry::Line2D transformLine(const geometry::Line2D& l) const noexcept;
};

}  // namespace projective