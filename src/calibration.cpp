#include "algorithms/calibration.hpp"

namespace algorithms {

Calibration::Calibration(const cv::Size& imageSize,
                         const std::vector<std::vector<cv::Point2f>>& imagePoints,
                         const std::vector<std::vector<cv::Point3f>>& objectPoints,
                         const std::vector<std::filesystem::path>& imagePaths,
                         const std::filesystem::path& outputDir)
    : imageSize_(imageSize),
      imagePoints_(imagePoints),
      objectPoints_(objectPoints),
      imagePaths_(imagePaths),
      outputDir_(outputDir) {}

void Calibration::calibrateCamera() {
    cameraMatrix_ = cv::Mat::eye(3, 3, CV_64F);

    const double rms = cv::calibrateCamera(objectPoints_, imagePoints_, imageSize_, cameraMatrix_,
                                           distCoeffs_, rvecs_, tvecs_);

    std::cout << "\n";
    std::cout << "=================================\n";
    std::cout << "Calibration Results\n";
    std::cout << "=================================\n";

    std::cout << "Valid images: " << imagePoints_.size() << '\n';

    std::cout << "RMS reprojection error: " << rms << '\n';

    std::cout << "\nCamera Matrix K:\n" << cameraMatrix_ << '\n';

    std::cout << "\nDistortion Coefficients:\n" << distCoeffs_ << '\n';
}

void Calibration::validateReprojection() const {
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

        imageError /= static_cast<double>(projectedPoints.size());

        std::cout << "Image " << i << " mean error: " << imageError << " px\n";
    }

    std::cout << "\nGlobal mean reprojection error: "
              << totalError / static_cast<double>(totalPoints) << " px\n";
}

void Calibration::compareProjection() const {
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

        imageError /= static_cast<double>(objectPoints[i].size());

        std::cout << "Image " << i << " mean difference: " << imageError << " px\n";
    }

    std::cout << "\nGlobal mean difference: "
              << totalProjectionError / static_cast<double>(totalProjectionPoints) << " px\n";
}

void Calibration::sensitivityAnalysis() const {
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
}

void Calibration::saveVisualization() const {
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
}

}  // namespace algorithms
