#pragma once
#include <core/mat.hpp>
#include <geometry/types.hpp>

namespace transform {

    struct Transform2D {
        core::Mat3 H;

        static Transform2D translation(double tx, double ty) noexcept;
        static Transform2D rotation(double a);
        static Transform2D scale(double s) noexcept;

        geometry::Point2D apply(const geometry::Point2D& p) const noexcept;
    };

} // namespace transform