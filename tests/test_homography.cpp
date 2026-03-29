#include <gtest/gtest.h>
#include "../include/projective/homography.h"
#include "../include/geometry/operations.h"

TEST(HomographyTest, IncidencePreserved) {
    Point2D p{1,1,1};
    Line2D l{1,-1,0};

    EXPECT_TRUE(incidence(p,l));

    Homography H;

    H.H = {{
        {1,0,1},
        {0,1,2},
        {0,0,1}
    }};

    Point2D p2 = H.transformPoint(p);
    Line2D l2 = H.transformLine(l);

    EXPECT_TRUE(incidence(p2,l2));
}