#include <gtest/gtest.h>
#include <geometry/operations.hpp>
#include <core/constants.hpp>

TEST(GeometryTest, JoinPointsIncidence) {
    geometry::Point2D p1{0, 0, 1};
    geometry::Point2D p2{1, 1, 1};

    geometry::Line2D l = geometry::join(p1, p2);

    EXPECT_TRUE(geometry::incidence(p1, l));
    EXPECT_TRUE(geometry::incidence(p2, l));
}

TEST(GeometryTest, MeetLinesIntersection) {
    geometry::Line2D l1{1, 0, 0};
    geometry::Line2D l2{0, 1, 0};

    geometry::Point2D p = geometry::meet(l1, l2);
    p = geometry::normalize(p);

    EXPECT_NEAR(p.x, 0.0, core::kEps);
    EXPECT_NEAR(p.y, 0.0, core::kEps);
    EXPECT_NEAR(p.z, 1.0, core::kEps);
}

TEST(GeometryTest, JoinMeetConsistency) {
    geometry::Point2D p1{0, 0, 1};
    geometry::Point2D p2{1, 1, 1};

    geometry::Line2D l = geometry::join(p1, p2);

    geometry::Line2D l2{1, -1, 0};

    geometry::Point2D p = geometry::meet(l, l2);

    if (!geometry::isAtInfinity(p)) {
        p = geometry::normalize(p);
    }

    EXPECT_TRUE(geometry::incidence(p, l));
    EXPECT_TRUE(geometry::incidence(p, l2));
}

TEST(GeometryTest, ParallelLinesGoToInfinity) {
    geometry::Line2D l1{0, 1, -1};
    geometry::Line2D l2{0, 1, -2};

    geometry::Point2D p = geometry::meet(l1, l2);

    EXPECT_TRUE(geometry::isAtInfinity(p));
}

TEST(GeometryTest, NormalizeFinitePoint) {
    geometry::Point2D p{2, 4, 2};

    geometry::Point2D n = geometry::normalize(p);

    EXPECT_NEAR(n.x, 1.0, core::kEps);
    EXPECT_NEAR(n.y, 2.0, core::kEps);
    EXPECT_NEAR(n.z, 1.0, core::kEps);
}

TEST(GeometryTest, InfinityCheck) {
    geometry::Point2D p{1, 2, 0};

    EXPECT_TRUE(geometry::isAtInfinity(p));
}

TEST(GeometryTest, CrossSymmetry) {
    geometry::Point2D p1{0, 0, 1};
    geometry::Point2D p2{1, 1, 1};

    geometry::Line2D l = geometry::join(p1, p2);
    geometry::Line2D l_rev = geometry::join(p2, p1);

    EXPECT_NEAR(l.x, -l_rev.x, core::kEps);
    EXPECT_NEAR(l.y, -l_rev.y, core::kEps);
    EXPECT_NEAR(l.z, -l_rev.z, core::kEps);
}
