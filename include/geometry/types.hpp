#pragma once
#include <core/vec.hpp>

namespace geometry {

/// @brief Point in homogeneous coordinates (x, y, w).
using Point2D = core::Vec3;

/// @brief Line in homogeneous form ax + by + c = 0.
using Line2D = core::Vec3;

}  // namespace geometry