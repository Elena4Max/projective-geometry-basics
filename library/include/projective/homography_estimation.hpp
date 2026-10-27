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
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& correspondences) noexcept;

/// @brief Normalizes points for numerically stable homography estimation.
/// @param pts Input points in homogeneous coordinates.
/// @return Pair of (normalized points, normalization transform T).
std::pair<std::vector<geometry::Point2D>, core::Mat3> normalizePoints(
    const std::vector<geometry::Point2D>& pts) noexcept;

/// @brief Estimates homography using RANSAC
core::Mat3 estimateHomographyRansac(
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& corr, double threshold,
    int iterations) noexcept;

/// @brief Computes Euclidean reprojection error between transformed point p and target point q.
/// @param H Homography matrix.
/// @param p Source point in homogeneous coordinates.
/// @param q Target point in homogeneous coordinates.
/// @return Reprojection error in pixels.
static double reprojectionError(const core::Mat3& H, const geometry::Point2D& p,
                                const geometry::Point2D& q) noexcept;

}  // namespace projective