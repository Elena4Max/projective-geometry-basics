#include <gtest/gtest.h>
#include <projective/homography.hpp>
#include <geometry/operations.hpp>

TEST(HomographyTest, IncidencePreserved) {
    geometry::Point2D p{1,1,1};
    geometry::Line2D l{1,-1,0};

    EXPECT_TRUE(geometry::incidence(p,l));

    projective::Homography H;

    H.H = {{
        {1,0,1},
        {0,1,2},
        {0,0,1}
    }};

    geometry::Point2D p2 = H.transformPoint(p);
    geometry::Line2D l2 = H.transformLine(l);

    EXPECT_TRUE(geometry::incidence(p2,l2));
}