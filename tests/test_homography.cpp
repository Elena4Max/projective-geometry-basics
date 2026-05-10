#include <gtest/gtest.h>

#include <geometry/operations.hpp>
#include <projective/homography.hpp>

TEST(HomographyTest, IncidencePreserved) {
    geometry::Point2D p{1, 1, 1};
    geometry::Line2D l{1, -1, 0};

    EXPECT_TRUE(geometry::incidence(p, l));

    projective::Homography H;

    H.H = core::Mat3(1, 0, 1, 0, 1, 2, 0, 0, 1);

    auto p2 = H.H * p;

    auto l2 = H.transformLine(l);

    EXPECT_TRUE(geometry::incidence(geometry::normalize(p2), l2));
}