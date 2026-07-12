#include <filesystem>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "algorithms/chessboard_detector.hpp"
#include "algorithms/frame_selector.hpp"
#include "algorithms/calibration.hpp"
#include "camera/camera_extrinsics.hpp"
#include "camera/camera_intrinsics.hpp"
#include "camera/image_directory.hpp"
#include "camera/projection.hpp"
#include "camera/video_source.hpp"
#include "core/mat.hpp"
#include "core/vec.hpp"
#include "geometry/types.hpp"
#ifdef HAVE_LIBCAMERA
#include "camera/libcamera_camera.hpp"
#endif

namespace fs = std::filesystem;

static std::vector<cv::Point3f> createChessboardPoints(int cols, int rows, float squareSize) {
    std::vector<cv::Point3f> points;
    points.reserve(static_cast<std::vector<cv::Point3f>::size_type>(cols) *
                   static_cast<std::vector<cv::Point3f>::size_type>(rows));

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            points.emplace_back(x * squareSize, y * squareSize, 0.0f);
        }
    }

    return points;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage:\n"
          << "  ./visualize images <directory>\n"
          << "  ./visualize video <video>\n";
        #ifdef HAVE_LIBCAMERA
        std::cerr << "  ./visualize camera <camera-id>\n";
        #endif

        return 1;
    }

    const std::string mode = argv[1];
    const fs::path input = argv[2];

    constexpr int cols = 6;
    constexpr int rows = 9;
    constexpr float squareSize = 1.0f;

    const cv::Size patternSize(cols, rows);

    algorithms::ChessboardDetector detector(patternSize);
    algorithms::FrameSelector selector(30);

    const auto boardPoints = createChessboardPoints(cols, rows, squareSize);

    std::vector<std::vector<cv::Point2f>> imagePoints;

    std::vector<std::vector<cv::Point3f>> objectPoints;

    std::vector<fs::path> imagePaths;

    cv::Size imageSize;

    const auto outputDir = input.parent_path() / "visualization";

    fs::create_directories(outputDir);

    auto processFrame = [&](const camera::Frame& frame) {
        const auto& image = frame.image;

        imageSize = image.size();

        const auto detection = detector.detect(frame);

        if (!detection.found) {
            std::cout << "[FAILED] " << frame.source.filename() << '\n';

            return;
        }

        if (!selector.accept(frame, detection)) {
            return;
        }

        imagePoints.push_back(detection.corners);

        objectPoints.push_back(boardPoints);

        imagePaths.push_back(frame.source);

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

            if (selector.finished()) {
                break;
            }
        }
    } else if (mode == "video") {
        camera::VideoSource source(input);

        while (auto frame = source.nextFrame()) {
            processFrame(*frame);

            if (selector.finished()) {
                break;
            }
        }
    #ifdef HAVE_LIBCAMERA
    } else if (mode == "camera") {
        camera::LibcameraCamera source(std::stoi(input.string()));

        if (!source.open()) {
            std::cerr << "Failed to open camera\n";
            return 1;
        }

        while (auto frame = source.nextFrame()) {
            processFrame(*frame);

            if (selector.finished()) {
                break;
            }
        }

        source.close();
    #endif
    } else {
        std::cerr << "Unknown mode\n";
        return 1;
    }

    if (imagePoints.empty()) {
        std::cerr << "No valid images found.\n";
        return 1;
    }

    algorithms::Calibration calibration(imageSize, imagePoints, objectPoints, imagePaths,
                                        outputDir);

    calibration.calibrateCamera();
    calibration.validateReprojection();
    calibration.compareProjection();
    calibration.sensitivityAnalysis();
    calibration.saveVisualization();

    return 0;
}