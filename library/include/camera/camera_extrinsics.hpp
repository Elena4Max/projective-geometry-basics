#pragma once

#include <core/mat.hpp>
#include <core/vec.hpp>

namespace camera {

/// @brief Camera pose in world coordinates.
/// @details Stores camera extrinsic parameters:
/// rotation matrix R and translation vector t.
struct CameraExtrinsics {
    core::Mat3 R;
    core::Vec3 t;
};

}  // namespace camera
