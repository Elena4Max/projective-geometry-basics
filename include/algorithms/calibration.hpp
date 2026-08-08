#pragma once

#include <filesystem>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "camera/camera_extrinsics.hpp"
#include "camera/camera_intrinsics.hpp"
#include "camera/projection.hpp"

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

   private:
    cv::Size imageSize;

    const std::vector<std::vector<cv::Point2f>>& imagePoints;

    const std::vector<std::vector<cv::Point3f>>& objectPoints;

    const std::vector<std::filesystem::path>& imagePaths;

    std::filesystem::path outputDir;

    cv::Mat cameraMatrix;

    cv::Mat distCoeffs;

    std::vector<cv::Mat> rvecs;

    std::vector<cv::Mat> tvecs;

    double rms{0.0};
};

}  // namespace algorithms