#pragma once
#include <core/mat.hpp>
#include <geometry/types.hpp>

namespace projective {

    struct Homography {
        core::Mat3 H;

        geometry::Point2D transformPoint(const geometry::Point2D& p) const noexcept;
        geometry::Line2D transformLine(const geometry::Line2D& l) const;
    };

} // namespace projective