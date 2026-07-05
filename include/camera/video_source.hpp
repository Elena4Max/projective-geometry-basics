#pragma once

#include <filesystem>
#include <optional>

#include <opencv2/videoio.hpp>

#include "camera/frame.hpp"

namespace camera
{

class VideoSource
{
public:
    explicit VideoSource(const std::filesystem::path& path);

    std::optional<Frame> nextFrame();

private:
    cv::VideoCapture capture_;
    std::filesystem::path source_;
    std::size_t sequence_ = 0;
};

}