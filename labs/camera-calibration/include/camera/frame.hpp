#pragma once

#include <cstdint>
#include <filesystem>
#include <opencv2/core/mat.hpp>

namespace camera {

struct Frame {
    cv::Mat image;
    uint64_t sequence = 0;
    std::filesystem::path source;
};

}  // namespace camera
