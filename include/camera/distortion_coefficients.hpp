#pragma once

namespace camera {

struct DistortionCoefficients {
    double k1{0.0};
    double k2{0.0};
    double p1{0.0};
    double p2{0.0};
    double k3{0.0};
};

} // namespace camera