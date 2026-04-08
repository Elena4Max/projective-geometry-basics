#include <projective/homography.hpp>
#include <cassert>

namespace projective {

    geometry::Point2D Homography::transformPoint(const geometry::Point2D& p) const noexcept{
        return H * p;
    }

    geometry::Line2D Homography::transformLine(const geometry::Line2D& l) const noexcept{
        assert(H.isInvertible() && "Homography matrix is not invertible");
        auto invT = H.inverse().transpose();
        return invT * l;
    }

} // namespace projective