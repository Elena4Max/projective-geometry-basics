#include <cassert>
#include <projective/homography.hpp>

namespace projective {

geometry::Line2D Homography::transformLine(const geometry::Line2D& l) const noexcept {
    assert(H.isInvertible() && "Homography matrix is not invertible");
    auto invT = H.inverse().transpose();
    return invT * l;
}

}  // namespace projective