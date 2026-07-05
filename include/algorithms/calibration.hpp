#pragma once

#include <filesystem>
#include <opencv2/core.hpp>
#include <vector>

namespace algorithms {

class Calibration {
   public:
    Calibration(const cv::Size& imageSize, const std::vector<std::vector<cv::Point2f>>& imagePoints,
                const std::vector<std::vector<cv::Point3f>>& objectPoints,
                const std::vector<std::filesystem::path>& imagePaths,
                const std::filesystem::path& outputDir);

    void calibrateCamera();

    void validateReprojection() const;

    void compareProjection() const;

    void sensitivityAnalysis() const;

    void saveVisualization() const;

   private:
    cv::Size imageSize_;

    const std::vector<std::vector<cv::Point2f>>& imagePoints_;

    const std::vector<std::vector<cv::Point3f>>& objectPoints_;

    const std::vector<std::filesystem::path>& imagePaths_;

    std::filesystem::path outputDir_;

    cv::Mat cameraMatrix_;

    cv::Mat distCoeffs_;

    std::vector<cv::Mat> rvecs_;

    std::vector<cv::Mat> tvecs_;
};

}  // namespace algorithms