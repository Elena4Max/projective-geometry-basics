#include <filesystem>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#include "camera/camera_extrinsics.hpp"
#include "camera/camera_intrinsics.hpp"
#include "camera/image_directory.hpp"
#include "camera/projection.hpp"
#include "core/mat.hpp"
#include "core/vec.hpp"
#include "geometry/types.hpp"

namespace fs = std::filesystem;

double projectionDifference(const core::Vec3& X, const camera::CameraIntrinsics& referenceK,
                            const camera::CameraIntrinsics& perturbedK,
                            const camera::CameraExtrinsics& E) {
    const auto reference = camera::projectPoint(X, referenceK, E);

    const auto perturbed = camera::projectPoint(X, perturbedK, E);

    return geometry::euclideanDistance(reference, perturbed);
}

static std::vector<cv::Point3f> createChessboardPoints(int cols, int rows, float squareSize) {
    std::vector<cv::Point3f> points;
    points.reserve(cols * rows);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            points.emplace_back(x * squareSize, y * squareSize, 0.0f);
        }
    }

    return points;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./visualize_calibration <images_dir>\n";
        return 1;
    }

    const fs::path imagesDir(argv[1]);

    constexpr int cols = 6;
    constexpr int rows = 9;
    constexpr float squareSize = 1.0f;

    const cv::Size patternSize(cols, rows);

    const auto boardPoints = createChessboardPoints(cols, rows, squareSize);

    std::vector<std::vector<cv::Point2f>> imagePoints;

    std::vector<std::vector<cv::Point3f>> objectPoints;

    std::vector<fs::path> imagePaths;

    cv::Size imageSize;

    const auto outputDir = imagesDir / "visualization";

    fs::create_directories(outputDir);

    camera::ImageDirectory source(imagesDir);

    while (auto frame = source.nextFrame()) {
        const auto& image = frame->image;

        imageSize = image.size();

        cv::Mat gray;

        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;

        const bool found = cv::findChessboardCornersSB(gray, patternSize, corners);

        if (!found) {
            std::cout << "[FAILED] " << frame->source.filename() << '\n';
            continue;
        }

        imagePoints.push_back(corners);
        objectPoints.push_back(boardPoints);
        imagePaths.push_back(frame->source);

        auto vis = image.clone();

        cv::drawChessboardCorners(vis, patternSize, corners, true);

        cv::imwrite((outputDir / ("corners_" + frame->source.filename().string())).string(), vis);

        std::cout << "[OK] " << frame->source.filename() << '\n';
    }

    if (imagePoints.empty()) {
        std::cerr << "No valid images found.\n";
        return 1;
    }

    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);

    cv::Mat distCoeffs;

    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;

    const double rms = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                                           distCoeffs, rvecs, tvecs);

    std::cout << "\n";
    std::cout << "=================================\n";
    std::cout << "Calibration Results\n";
    std::cout << "=================================\n";

    std::cout << "Valid images: " << imagePoints.size() << '\n';

    std::cout << "RMS reprojection error: " << rms << '\n';

    std::cout << "\nCamera Matrix K:\n" << cameraMatrix << '\n';

    std::cout << "\nDistortion Coefficients:\n" << distCoeffs << '\n';

    std::cout << "\n=================================\n"
              << "Reprojection Validation\n"
              << "=================================\n";

    double totalError = 0.0;
    std::size_t totalPoints = 0;

    for (std::size_t i = 0; i < objectPoints.size(); ++i) {
        std::vector<cv::Point2f> projectedPoints;

        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs,
                          projectedPoints);

        double imageError = 0.0;

        for (std::size_t j = 0; j < projectedPoints.size(); ++j) {
            const double dx = projectedPoints[j].x - imagePoints[i][j].x;

            const double dy = projectedPoints[j].y - imagePoints[i][j].y;

            const double error = std::sqrt(dx * dx + dy * dy);

            imageError += error;
            totalError += error;
            ++totalPoints;
        }

        imageError /= projectedPoints.size();

        std::cout << "Image " << i << " mean error: " << imageError << " px\n";
    }

    std::cout << "\nGlobal mean reprojection error: " << totalError / totalPoints << " px\n";

    std::cout << "\n=================================\n"
              << "OpenCV vs Custom Projection\n"
              << "=================================\n";

    double totalProjectionError = 0.0;
    std::size_t totalProjectionPoints = 0;

    for (std::size_t i = 0; i < objectPoints.size(); ++i) {
        std::vector<cv::Point2f> cvProjectedPoints;

        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, cv::Mat(),
                          cvProjectedPoints);

        cv::Mat cvR;

        cv::Rodrigues(rvecs[i], cvR);

        camera::CameraIntrinsics K{cameraMatrix.at<double>(0, 0), cameraMatrix.at<double>(1, 1),
                                   cameraMatrix.at<double>(0, 2), cameraMatrix.at<double>(1, 2)};

        camera::CameraExtrinsics E{
            core::Mat3{cvR.at<double>(0, 0), cvR.at<double>(0, 1), cvR.at<double>(0, 2),
                       cvR.at<double>(1, 0), cvR.at<double>(1, 1), cvR.at<double>(1, 2),
                       cvR.at<double>(2, 0), cvR.at<double>(2, 1), cvR.at<double>(2, 2)},
            core::Vec3{tvecs[i].at<double>(0), tvecs[i].at<double>(1), tvecs[i].at<double>(2)}};

        double imageError = 0.0;

        for (std::size_t j = 0; j < objectPoints[i].size(); ++j) {
            const auto& p = objectPoints[i][j];

            const core::Vec3 X{p.x, p.y, p.z};

            const auto customProjected = camera::projectPoint(X, K, E);

            geometry::Point2D cvPoint{cvProjectedPoints[j].x, cvProjectedPoints[j].y, 1.0};

            const double error = camera::reprojectionError(cvPoint, customProjected);

            imageError += error;
            totalProjectionError += error;
            ++totalProjectionPoints;
        }

        imageError /= objectPoints[i].size();

        std::cout << "Image " << i << " mean difference: " << imageError << " px\n";
    }

    std::cout << "\nGlobal mean difference: " << totalProjectionError / totalProjectionPoints
              << " px\n";

    std::cout << "\n=================================\n"
              << "Sensitivity Analysis\n"
              << "=================================\n";

    cv::Mat cvR;

    cv::Rodrigues(rvecs.front(), cvR);

    camera::CameraIntrinsics K{cameraMatrix.at<double>(0, 0), cameraMatrix.at<double>(1, 1),
                               cameraMatrix.at<double>(0, 2), cameraMatrix.at<double>(1, 2)};

    camera::CameraExtrinsics E{
        core::Mat3{cvR.at<double>(0, 0), cvR.at<double>(0, 1), cvR.at<double>(0, 2),
                   cvR.at<double>(1, 0), cvR.at<double>(1, 1), cvR.at<double>(1, 2),
                   cvR.at<double>(2, 0), cvR.at<double>(2, 1), cvR.at<double>(2, 2)},
        core::Vec3{tvecs.front().at<double>(0), tvecs.front().at<double>(1),
                   tvecs.front().at<double>(2)}};

    auto meanError = [&](auto perturb) {
        double totalError = 0.0;
        std::size_t totalPoints = 0;

        auto modifiedK = K;

        perturb(modifiedK);

        for (const auto& p : objectPoints.front()) {
            const core::Vec3 X{p.x, p.y, p.z};

            totalError += projectionDifference(X, K, modifiedK, E);

            ++totalPoints;
        }

        return totalError / static_cast<double>(totalPoints);
    };

    std::cout << "\nParameter Change\tMean Error [px]\n";

    std::cout << "fx +1%\t\t" << meanError([](auto& K) { K.fx *= 1.01; }) << '\n';

    std::cout << "fx +5%\t\t" << meanError([](auto& K) { K.fx *= 1.05; }) << '\n';

    std::cout << "fx +10%\t\t" << meanError([](auto& K) { K.fx *= 1.10; }) << '\n';

    std::cout << "cx +1 px\t" << meanError([](auto& K) { K.cx += 1.0; }) << '\n';

    std::cout << "cx +5 px\t" << meanError([](auto& K) { K.cx += 5.0; }) << '\n';

    std::cout << "cx +20 px\t" << meanError([](auto& K) { K.cx += 20.0; }) << '\n';

    const auto image = cv::imread(imagePaths.front().string());

    std::vector<cv::Point2f> projectedPoints;

    cv::projectPoints(objectPoints.front(), rvecs.front(), tvecs.front(), cameraMatrix, distCoeffs,
                      projectedPoints);

    auto vis = image.clone();

    for (const auto& p : imagePoints.front()) {
        cv::circle(vis, p, 5, cv::Scalar(0, 255, 0), -1);
    }

    for (const auto& p : projectedPoints) {
        cv::circle(vis, p, 3, cv::Scalar(0, 0, 255), -1);
    }

    cv::imwrite((outputDir / "reprojection_validation.png").string(), vis);

    return 0;
}