#include <camera/projection.hpp>
#include <camera/rotation.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
using camera::CameraIntrinsics;

void drawPoint(cv::Mat& img, const geometry::Point2D& p,
               const cv::Scalar& color = cv::Scalar(0, 0, 255)) {
    cv::circle(img, cv::Point(static_cast<int>(p.x), static_cast<int>(p.y)), 5, color, -1);
}

void drawLine(cv::Mat& img, const geometry::Point2D& p1, const geometry::Point2D& p2,
              const cv::Scalar& color = cv::Scalar(0, 0, 0)) {
    cv::line(img, cv::Point(static_cast<int>(p1.x), static_cast<int>(p1.y)),
             cv::Point(static_cast<int>(p2.x), static_cast<int>(p2.y)), color, 2);
}

int main() {
    cv::Mat canvas(1500, 1500, CV_8UC3, cv::Scalar(255, 255, 255));

    CameraIntrinsics K{500.0, 500.0, 400.0, 400.0};

    camera::CameraExtrinsics E{camera::rotationY(0.5) * camera::rotationX(0.3), {0.0, 0.0, 0.0}};

    std::vector<core::Vec3> cube = {

        {-1.0, -1.0, 2.0}, {1.0, -1.0, 2.0}, {1.0, 1.0, 2.0}, {-1.0, 1.0, 2.0},

        {-1.0, -1.0, 4.0}, {1.0, -1.0, 4.0}, {1.0, 1.0, 4.0}, {-1.0, 1.0, 4.0},
    };

    std::vector<geometry::Point2D> projected;

    for (const auto& p : cube) {
        auto x = camera::projectPoint(p, K, E);

        projected.push_back(x);

        drawPoint(canvas, x);
    }

    const std::vector<std::pair<int, int>> edges = {

        {0, 1}, {1, 2}, {2, 3}, {3, 0},

        {4, 5}, {5, 6}, {6, 7}, {7, 4},

        {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    for (const auto& [a, b] : edges) {
        drawLine(canvas, projected[a], projected[b]);
    }

    cv::imwrite("camera_projection.jpg", canvas);

    std::cout << "Saved camera_projection.jpg\n";

    return 0;
}
