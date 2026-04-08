#pragma once
#include <cmath>

namespace core {

struct Vec3 {
    double x{0.0}, y{0.0}, z{0.0};

    constexpr Vec3() noexcept = default;
    constexpr Vec3(double x_, double y_, double z_) noexcept : x(x_), y(y_), z(z_) {}

    constexpr Vec3 operator+(const Vec3& o) const noexcept { return {x + o.x, y + o.y, z + o.z}; }

    constexpr Vec3 operator-(const Vec3& o) const noexcept { return {x - o.x, y - o.y, z - o.z}; }

    constexpr Vec3 operator*(double s) const noexcept { return {x * s, y * s, z * s}; }
};

}  // namespace core