#include <cmath>
#include <transform/transform2d.hpp>

namespace transform {

Transform2D Transform2D::translation(double tx, double ty) noexcept {
    Transform2D t;

    t.H = core::Mat3(1, 0, tx, 0, 1, ty, 0, 0, 1);

    return t;
}

Transform2D Transform2D::rotation(double a) noexcept {
    double c = std::cos(a);
    double s = std::sin(a);

    Transform2D t;

    t.H = core::Mat3(c, -s, 0, s, c, 0, 0, 0, 1);

    return t;
}

Transform2D Transform2D::scale(double s) noexcept {
    Transform2D t;

    t.H = core::Mat3(s, 0, 0, 0, s, 0, 0, 0, 1);

    return t;
}

geometry::Point2D Transform2D::apply(const geometry::Point2D& p) const noexcept { return H * p; }

}  // namespace transform