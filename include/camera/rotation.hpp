#pragma once

#include <core/mat.hpp>

namespace camera {

/// @brief Creates a rotation matrix around the X axis.
/// @param angle Rotation angle in radians.
/// @return 3x3 rotation matrix.
core::Mat3 rotationX(double angle) noexcept;

/// @brief Creates a rotation matrix around the Y axis.
/// @param angle Rotation angle in radians.
/// @return 3x3 rotation matrix.
core::Mat3 rotationY(double angle) noexcept;

/// @brief Creates a rotation matrix around the Z axis.
/// @param angle Rotation angle in radians.
/// @return 3x3 rotation matrix.
core::Mat3 rotationZ(double angle) noexcept;

}  // namespace camera
