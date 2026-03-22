#include <opencv2/opencv.hpp>
#include "../src/geometry.h"
#include <iostream>

constexpr double EPS = 1e-6;

cv::Point2d toCartesian(const Point& p) {
    return {p.x / p.z, p.y / p.z};
}

void drawPoint(cv::Mat& img, const Point& p, cv::Scalar color) {
    if (isAtInfinity(p)) return;

    Point pn = normalize(p);
    auto pt = toCartesian(pn);

    cv::circle(img, pt, 5, color, -1);
}

void drawLine(cv::Mat& img, const Line& l, cv::Scalar color) {
    int w = img.cols;
    int h = img.rows;

    if (std::abs(l.y) > EPS) {
        double y1 = -(l.x * 0 + l.z) / l.y;
        double y2 = -(l.x * w + l.z) / l.y;

        cv::line(img, {0, (int)y1}, {w, (int)y2}, color, 2);
    } else {
        double x = -l.z / l.x;
        cv::line(img, {(int)x, 0}, {(int)x, h}, color, 2);
    }
}

cv::Mat createCanvas() {
    return cv::Mat(500, 500, CV_8UC3, cv::Scalar(255,255,255));
}

void save(const cv::Mat& img, const std::string& name) {
    cv::imwrite(name, img);
    std::cout << "Saved: " << name << "\n";
}

void testIntersection() {
    auto img = createCanvas();

    Line l1{0, 1, -100}; // y = 100
    Line l2{1, -1, 0};   // x - y = 0

    drawLine(img, l1, {255, 0, 0});
    drawLine(img, l2, {0, 255, 0});

    Point p = meet(l1, l2);
    drawPoint(img, p, {0, 0, 255});

    save(img, "intersection.png");
}

void testParallel() {
    auto img = createCanvas();

    Line l1{0, 1, -100}; // y = 100
    Line l2{0, 1, -200}; // y = 200

    drawLine(img, l1, {255, 0, 0});
    drawLine(img, l2, {0, 255, 0});

    Point p = meet(l1, l2);

    if (isAtInfinity(p)) {
        std::cout << "Parallel lines → intersection at infinity\n";
    } else {
        drawPoint(img, p, {0,0,255});
    }

    save(img, "parallel.png");
}

void testJoin() {
    auto img = createCanvas();

    Point p1{100, 100, 1};
    Point p2{400, 300, 1};

    Line l = join(p1, p2);

    drawLine(img, l, {255, 0, 0});
    drawPoint(img, p1, {0, 255, 0});
    drawPoint(img, p2, {0, 255, 0});

    save(img, "join.png");
}

void testEuclideanVsProjective() {
    int W = 1000;
    int H = 500;

    cv::Mat img(H, W, CV_8UC3, cv::Scalar(255,255,255));

    int offset = 0;

    Line l1{0, 1, -100}; // y=100
    Line l2{0, 1, -200}; // y=200

    drawLine(img, l1, {255, 0, 0});
    drawLine(img, l2, {0, 200, 0});

    cv::putText(img, "Euclidean Geometry",
                {50, 30}, cv::FONT_HERSHEY_SIMPLEX, 0.7, {0,0,0}, 2);

    cv::putText(img, "No intersection",
                {50, 60}, cv::FONT_HERSHEY_SIMPLEX, 0.6, {0,0,255}, 1);

    offset = 500;

    Line l3{0, 1, -100};
    Line l4{0, 1, -200};

    Line l3_shifted{l3.x, l3.y, l3.z - offset * l3.x};
    Line l4_shifted{l4.x, l4.y, l4.z - offset * l4.x};

    drawLine(img, l3_shifted, {255, 0, 0});
    drawLine(img, l4_shifted, {0, 200, 0});

    cv::putText(img, "Projective Geometry",
                {550, 30}, cv::FONT_HERSHEY_SIMPLEX, 0.7, {0,0,0}, 2);

    cv::putText(img, "Intersection at infinity",
                {550, 60}, cv::FONT_HERSHEY_SIMPLEX, 0.6, {0,0,255}, 1);

    cv::putText(img, "(w ≈ 0)",
                {550, 90}, cv::FONT_HERSHEY_SIMPLEX, 0.6, {0,0,255}, 1);

    save(img, "euclidean_vs_projective.png");
}

int main() {
    testIntersection();
    testParallel();
    testJoin();
    testEuclideanVsProjective();

    return 0;
}
