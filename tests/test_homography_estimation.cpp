#include <gtest/gtest.h>

#include <projective/homography.hpp>
#include <projective/homography_estimation.hpp>

using core::Mat3;
using geometry::Point2D;
using projective::estimateHomography;

static Point2D normalize(const Point2D& p) {
    if (std::abs(p.z) < 1e-9) {
        return p;
    }
    return {p.x / p.z, p.y / p.z, 1.0};
}

TEST(HomographyEstimation, RecoversKnownTransform) {
    Mat3 H_true(1.0, 0.2, 50.0, 0.1, 1.0, 30.0, 0.0005, 0.0008, 1.0);

    std::vector<Point2D> pts = {{0, 0, 1},     {100, 0, 1}, {0, 100, 1},
                                {100, 100, 1}, {50, 20, 1}, {30, 70, 1}};

    std::vector<std::pair<Point2D, Point2D>> correspondences;

    for (const auto& p : pts) {
        Point2D p2 = H_true * p;
        p2 = normalize(p2);
        correspondences.emplace_back(p, p2);
    }

    Mat3 H_est = estimateHomography(correspondences);

    for (const auto& p : pts) {
        Point2D p_true = normalize(H_true * p);
        Point2D p_est = normalize(H_est * p);

        EXPECT_NEAR(p_true.x, p_est.x, 1e-6);
        EXPECT_NEAR(p_true.y, p_est.y, 1e-6);
    }
}

TEST(HomographyEstimation, Identity) {
    Mat3 H = Mat3::identity();

    std::vector<std::pair<Point2D, Point2D>> corr = {
        {{0, 0, 1}, {0, 0, 1}},
        {{1, 0, 1}, {1, 0, 1}},
        {{0, 1, 1}, {0, 1, 1}},
        {{1, 1, 1}, {1, 1, 1}},
    };

    Mat3 H_est = estimateHomography(corr);

    for (const auto& [p, _] : corr) {
        auto p_est = normalize(H_est * p);
        auto p_true = normalize(p);

        EXPECT_NEAR(p_true.x, p_est.x, 1e-6);
        EXPECT_NEAR(p_true.y, p_est.y, 1e-6);
    }
}