#pragma once
#include <limits>

namespace core {

/// @brief Epsilon for zero checks in geometric computations.
inline constexpr double kEps = 1e-9;

/// @brief Large finite value used to represent invalid or infinite error.
inline constexpr double kInf = std::numeric_limits<double>::max();

}  // namespace core