#pragma once

#include <camera/camera_extrinsics.hpp>
#include <camera/camera_intrinsics.hpp>
#include <geometry/operations.hpp>

namespace camera {

/// @brief Projects a 3D point into image coordinates.
/// @param X 3D point in camera coordinates.
/// @param K Camera intrinsic parameters.
/// @return Projected 2D image point.
geometry::Point2D projectPoint(const core::Vec3& X, const CameraIntrinsics& K) noexcept;

/// @brief Projects a 3D world point using full camera pose.
/// @param X 3D point in world coordinates.
/// @param K Camera intrinsic parameters.
/// @param E Camera extrinsic parameters.
/// @return Projected 2D image point.
geometry::Point2D projectPoint(const core::Vec3& X, const CameraIntrinsics& K,
                               const CameraExtrinsics& E) noexcept;

}  // namespace camera
