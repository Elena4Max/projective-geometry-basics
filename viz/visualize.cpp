#include <opencv2/opencv.hpp>
#include <geometry/operations.hpp>
#include <projective/homography.hpp>
#include <iostream>

#include <core/constants.hpp>

cv::Point2d toCartesian(
    geometry::Point2D& p) {
    return {p.x / p.z, p.y / p.z};
}

void drawText(cv::Mat& img, const std::string& text, int x, int y) {
    cv::putText(img, text, {x,y},
        cv::FONT_HERSHEY_SIMPLEX, 0.6, {0,0,0}, 1);
}

void drawPoint(cv::Mat& img, const geometry::Point2D& p, const cv::Scalar& color, int offsetX = 0) {
    if (geometry::isAtInfinity(p)) {
        return;
    }

    geometry::Point2D pn = geometry::normalize(p);
    cv::circle(img,
        {offsetX + (int)pn.x, (int)pn.y},
        5, color, -1);
}

void drawLine(cv::Mat& img, const geometry::Line2D& l, const cv::Scalar& color, int offsetX = 0) {
    int w = img.cols;
    int h = img.rows;

    if (std::abs(l.y) > core::kEps) {
        double y1 = -(l.x * 0 + l.z) / l.y;
        double y2 = -(l.x * w + l.z) / l.y;

        cv::line(img,
            {offsetX + 0, (int)y1},
            {offsetX + w, (int)y2},
            color, 2);
    } else {
        double x = -l.z / l.x;
        cv::line(img,
            {offsetX + (int)x, 0},
            {offsetX + (int)x, h},
            color, 2);
    }
}

cv::Mat createCanvas(int w = 500, int h = 500) {
    return cv::Mat(h, w, CV_8UC3, cv::Scalar(255,255,255));
}

void save(const cv::Mat& img, const std::string& name) {
    cv::imwrite("images/" + name, img);
    std::cout << "Saved: images/" << name << "\n";
}

void testIntersection() {
    auto img = createCanvas();

    drawText(img, "Intersection", 20, 30);

    geometry::Line2D l1{0, 1, -100};
    geometry::Line2D l2{1, -1, 0};

    drawLine(img, l1, {255,0,0});
    drawLine(img, l2, {0,200,0});

    geometry::Point2D p = geometry::meet(l1, l2);
    drawPoint(img, p, {0,0,255});

    save(img, "intersection.png");
}

void testParallel() {
    auto img = createCanvas();

    drawText(img, "Parallel lines", 20, 30);
    drawText(img, "Intersection at infinity", 20, 60);

    geometry::Line2D l1{0, 1, -100};
    geometry::Line2D l2{0, 1, -200};

    drawLine(img, l1, {255,0,0});
    drawLine(img, l2, {0,200,0});

    save(img, "parallel.png");
}

void testJoin() {
    auto img = createCanvas();

    drawText(img, "Line from two points", 20, 30);

    geometry::Point2D p1{100,100,1};
    geometry::Point2D p2{400,300,1};

    geometry::Line2D l = geometry::join(p1, p2);

    drawLine(img, l, {255,0,0});
    drawPoint(img, p1, {0,200,0});
    drawPoint(img, p2, {0,200,0});

    save(img, "join.png");
}

void testEuclideanVsProjective() {
    int width = 1000;
    int height = 500;
    cv::Mat img(height, width, CV_8UC3, cv::Scalar(255,255,255));

    drawText(img, "Euclidean", 50, 30);
    drawText(img, "No intersection", 50, 60);

    geometry::Line2D l1{0,1,-100};
    geometry::Line2D l2{0,1,-200};

    drawLine(img, l1, {255,0,0}, 0);
    drawLine(img, l2, {0,200,0}, 0);

    int offset = width/2;

    drawText(img, "Projective", offset+50, 30);
    drawText(img, "Intersection at infinity", offset+50, 60);

    geometry::Line2D l3{0,1,-100};
    geometry::Line2D l4{0,1,-200};

    geometry::Line2D l3s{l3.x, l3.y, l3.z - offset*l3.x};
    geometry::Line2D l4s{l4.x, l4.y, l4.z - offset*l4.x};

    drawLine(img, l3s, {255,0,0}, offset);
    drawLine(img, l4s, {0,200,0}, offset);

    save(img, "euclidean_vs_projective.png");
}

void testHomography() {
    int width = 1000;
    int height = 500;
    cv::Mat img(height, width, CV_8UC3, cv::Scalar(255,255,255));

    geometry::Point2D p1{100,100,1};
    geometry::Point2D p2{400,300,1};

    geometry::Line2D l = geometry::join(p1, p2);

    projective::Homography Hm;
    Hm.H = core::Mat3(
        1,     0.2,   50,
        0.1,   1,     30,
        0.0005,0.0008,1
    );

    geometry::Point2D p1_t = Hm.transformPoint(p1);
    geometry::Point2D p2_t = Hm.transformPoint(p2);
    geometry::Line2D l_t = Hm.transformLine(l);

    drawText(img, "Original", 20, 30);
    drawLine(img, l, {255,0,0}, 0);
    drawPoint(img, p1, {0,200,0}, 0);
    drawPoint(img, p2, {0,200,0}, 0);

    int offset = width/2;

    drawText(img, "After Homography", offset+20, 30);
    drawText(img, "Incidence preserved", offset+20, 60);

    drawLine(img, l_t, {255,0,0}, offset);
    drawPoint(img, p1_t, {0,200,0}, offset);
    drawPoint(img, p2_t, {0,200,0}, offset);

    save(img, "homography.png");
}


int main() {
    testIntersection();
    testParallel();
    testJoin();
    testEuclideanVsProjective();
    testHomography();

    return 0;
}