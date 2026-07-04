#pragma once

#include <cstdint>
#include <filesystem>

#include <opencv2/core/mat.hpp>

namespace camera
{

enum class CameraId
{
    Left,
    Right,
    Unknown
};

struct Frame
{
    cv::Mat image;

    uint64_t timestamp = 0;

    uint64_t sequence = 0;

    CameraId camera = CameraId::Unknown;

    std::filesystem::path source;
};

}