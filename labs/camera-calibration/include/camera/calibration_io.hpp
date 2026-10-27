#pragma once

#include <filesystem>
#include <opencv2/core.hpp>

namespace camera {

class CalibrationIO {
   public:
    static bool save(const std::filesystem::path& filename, const cv::Mat& cameraMatrix,
                     const cv::Mat& distortion, const cv::Size& imageSize, double rms);

    static bool load(const std::filesystem::path& filename, cv::Mat& cameraMatrix,
                     cv::Mat& distortion, cv::Size& imageSize, double& rms);
};

}  // namespace camera