#include <filesystem>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <system_error>
#include <vector>

#include "algorithms/chessboard_detector.hpp"
#include "algorithms/calibration.hpp"
#include "camera/camera_extrinsics.hpp"
#include "camera/camera_intrinsics.hpp"
#include "camera/image_directory.hpp"
#include "camera/projection.hpp"
#include "camera/video_source.hpp"
#include "core/mat.hpp"
#include "core/vec.hpp"
#include "geometry/types.hpp"

namespace fs = std::filesystem;

static std::vector<cv::Point3f> createChessboardPoints(int cols, int rows, float squareSize) {
    std::vector<cv::Point3f> points;
    points.reserve(static_cast<std::vector<cv::Point3f>::size_type>(cols) *
                   static_cast<std::vector<cv::Point3f>::size_type>(rows));

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            points.emplace_back(
                static_cast<float>(x) * squareSize, 
                static_cast<float>(y) * squareSize, 
                0.0f);
        }
    }

    return points;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage:\n"
          << "  lab_camera_calibration images <directory>\n"
          << "  lab_camera_calibration video <video>\n";
        return 1;
    }

    const std::string mode = argv[1];
    const fs::path input = argv[2];

    if (mode != "images" && mode != "video") {
        std::cerr << "Unknown input mode: " << mode << '\n';
        return 1;
    }

    std::error_code pathError;
    if (mode == "images" && !fs::is_directory(input, pathError)) {
        std::cerr << "Input directory is missing or isn't a directory: " << input << '\n';
        return 1;
    }
    if (mode == "video" && !fs::is_regular_file(input, pathError)) {
        std::cerr << "Video file is missing or isn't a regular file: " << input << '\n';
        return 1;
    }

    constexpr int cols = 6;
    constexpr int rows = 9;
    constexpr float squareSize = 1.0f;

    const cv::Size patternSize(cols, rows);

    algorithms::ChessboardDetector detector(patternSize);
    constexpr std::size_t frameStep = 30;
    constexpr std::size_t maxFrames = 20;
    std::size_t acceptedFrames = 0;

    const auto boardPoints = createChessboardPoints(cols, rows, squareSize);

    std::vector<std::vector<cv::Point2f>> imagePoints;

    std::vector<std::vector<cv::Point3f>> objectPoints;

    cv::Size imageSize;

    const auto outputDir = fs::current_path() / "calibration-output";

    fs::create_directories(outputDir);

    auto processFrame = [&](const camera::Frame& frame) {
        const auto& image = frame.image;

        imageSize = image.size();

        const auto detection = detector.detect(frame);

        if (!detection.found) {
            std::cout << "[FAILED] " << frame.source.filename() << '\n';

            return;
        }

        if ((mode == "video" && frame.sequence % frameStep != 0) ||
            acceptedFrames >= maxFrames) {
            return;
        }

        ++acceptedFrames;

        imagePoints.push_back(detection.corners);

        objectPoints.push_back(boardPoints);

        auto vis = image.clone();

        cv::drawChessboardCorners(vis, patternSize, detection.corners, true);

        fs::path outputName;

        if (mode == "images") {
            outputName = "corners_" + frame.source.filename().string();
        } else {
            outputName = "corners_" + std::to_string(frame.sequence) + ".png";
        }

        cv::imwrite((outputDir / outputName).string(), vis);

        std::cout << "[OK] " << frame.source.filename() << '\n';
    };

    if (mode == "images") {
        camera::ImageDirectory source(input);

        while (auto frame = source.nextFrame()) {
            processFrame(*frame);

            if (acceptedFrames >= maxFrames) {
                break;
            }
        }
    } else if (mode == "video") {
        camera::VideoSource source(input);

        while (auto frame = source.nextFrame()) {
            processFrame(*frame);

            if (acceptedFrames >= maxFrames) {
                break;
            }
        }
    }

    if (imagePoints.empty()) {
        std::cerr << "No valid images found.\n";
        return 1;
    }

    algorithms::Calibration calibration(imageSize, imagePoints, objectPoints);

    calibration.calibrateCamera();
    calibration.validateReprojection();
    calibration.compareProjection();
    calibration.sensitivityAnalysis();

    return 0;
}
