#pragma once
#include <cmath>

namespace core {

/// @brief 3D homogeneous vector (x, y, w).
/// @note Used to represent both points and lines in projective geometry.
struct Vec3 {
    double x{0.0}, y{0.0}, z{0.0};

    /// @brief Default constructor (zero vector).
    constexpr Vec3() noexcept = default;
    /// @brief Constructs vector from components.
    constexpr Vec3(double x_, double y_, double z_) noexcept : x(x_), y(y_), z(z_) {}

    /// @brief Vector addition.
    constexpr Vec3 operator+(const Vec3& o) const noexcept { return {x + o.x, y + o.y, z + o.z}; }

    /// @brief Vector subtraction.
    constexpr Vec3 operator-(const Vec3& o) const noexcept { return {x - o.x, y - o.y, z - o.z}; }

    /// @brief Scalar multiplication.
    constexpr Vec3 operator*(double s) const noexcept { return {x * s, y * s, z * s}; }
};

}  // namespace core