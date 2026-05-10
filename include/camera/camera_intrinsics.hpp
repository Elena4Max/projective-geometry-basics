#pragma once

namespace camera {

/// @brief Camera intrinsic parameters.
/// @details Defines focal lengths and principal point
/// used for perspective projection.
struct CameraIntrinsics {
    double fx{0.0};
    double fy{0.0};
    double cx{0.0};
    double cy{0.0};
};

}  // namespace camera
