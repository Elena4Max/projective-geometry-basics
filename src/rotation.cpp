#include <camera/rotation.hpp>
#include <cmath>

namespace camera {

core::Mat3 rotationX(double angle) noexcept {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    return {1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c};
}

core::Mat3 rotationY(double angle) noexcept {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    return {c, 0.0, s, 0.0, 1.0, 0.0, -s, 0.0, c};
}

core::Mat3 rotationZ(double angle) noexcept {
    const double c = std::cos(angle);
    const double s = std::sin(angle);

    return {c, -s, 0.0, s, c, 0.0, 0.0, 0.0, 1.0};
}

}  // namespace camera