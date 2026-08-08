#include <gtest/gtest.h>

#include <filesystem>

#include "camera/calibration_io.hpp"

namespace fs = std::filesystem;

TEST(CalibrationIO, SaveAndLoad) {
    const fs::path filename = "camera.yml";

    cv::Mat K = (cv::Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);

    cv::Mat D = (cv::Mat_<double>(1, 5) << 0.1, -0.2, 0.01, 0.02, 0.3);

    const cv::Size imageSize(640, 480);

    constexpr double rms = 0.42;

    ASSERT_TRUE(camera::CalibrationIO::save(filename, K, D, imageSize, rms));

    cv::Mat loadedK;
    cv::Mat loadedD;
    cv::Size loadedSize;
    double loadedRms = 0.0;

    ASSERT_TRUE(camera::CalibrationIO::load(filename, loadedK, loadedD, loadedSize, loadedRms));

    EXPECT_EQ(cv::countNonZero(K != loadedK), 0);
    EXPECT_EQ(cv::countNonZero(D != loadedD), 0);

    EXPECT_EQ(imageSize.width, loadedSize.width);
    EXPECT_EQ(imageSize.height, loadedSize.height);

    EXPECT_DOUBLE_EQ(rms, loadedRms);

    fs::remove(filename);
}

TEST(CalibrationIO, LoadMissingFile) {
    cv::Mat K;
    cv::Mat D;
    cv::Size size;
    double rms = 0.0;

    EXPECT_FALSE(camera::CalibrationIO::load("missing.yml", K, D, size, rms));
}

TEST(CalibrationIO, SaveInvalidPath) {
    EXPECT_FALSE(camera::CalibrationIO::save("/not_existing_directory/camera.yml",
                                             cv::Mat::eye(3, 3, CV_64F), cv::Mat(),
                                             cv::Size(640, 480), 1.0));
}