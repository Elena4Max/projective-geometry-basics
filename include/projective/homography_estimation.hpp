#pragma once
#include <core/mat.hpp>
#include <geometry/operations.hpp>
#include <geometry/types.hpp>
#include <transform/transform2d.hpp>
#include <vector>

namespace projective {

/// @brief Estimates homography from point correspondences using DLT.
/// @param correspondences Pairs of corresponding points (p <-> p') in homogeneous coordinates.
/// @return 3x3 homography matrix H such that p' ≈ H * p.
/// @pre At least 4 correspondences are required.
core::Mat3 estimateHomography(
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& correspondences);

/// @brief Normalizes points for numerically stable homography estimation.
/// @param pts Input points in homogeneous coordinates.
/// @return Pair of (normalized points, normalization transform T).
std::pair<std::vector<geometry::Point2D>, core::Mat3> normalizePoints(
    const std::vector<geometry::Point2D>& pts);

}  // namespace projective