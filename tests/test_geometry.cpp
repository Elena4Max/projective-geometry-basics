#include <gtest/gtest.h>
#include "../include/geometry/operations.h"

constexpr double EPS = 1e-6;

TEST(GeometryTest, JoinPointsIncidence) {
    Point2D p1{0, 0, 1};
    Point2D p2{1, 1, 1};

    Line2D l = join(p1, p2);

    EXPECT_TRUE(incidence(p1, l));
    EXPECT_TRUE(incidence(p2, l));
}

TEST(GeometryTest, MeetLinesIntersection) {
    Line2D l1{1, 0, 0};
    Line2D l2{0, 1, 0};

    Point2D p = meet(l1, l2);
    p = normalize(p);

    EXPECT_NEAR(p.x, 0.0, EPS);
    EXPECT_NEAR(p.y, 0.0, EPS);
    EXPECT_NEAR(p.z, 1.0, EPS);
}

TEST(GeometryTest, JoinMeetConsistency) {
    Point2D p1{0, 0, 1};
    Point2D p2{1, 1, 1};

    Line2D l = join(p1, p2);

    Line2D l2{1, -1, 0};

    Point2D p = meet(l, l2);

    if (!isAtInfinity(p)) {
        p = normalize(p);
    }

    EXPECT_TRUE(incidence(p, l));
    EXPECT_TRUE(incidence(p, l2));
}

TEST(GeometryTest, ParallelLinesGoToInfinity) {
    Line2D l1{0, 1, -1};
    Line2D l2{0, 1, -2};

    Point2D p = meet(l1, l2);

    EXPECT_TRUE(isAtInfinity(p));
}

TEST(GeometryTest, NormalizeFinitePoint) {
    Point2D p{2, 4, 2};

    Point2D n = normalize(p);

    EXPECT_NEAR(n.x, 1.0, EPS);
    EXPECT_NEAR(n.y, 2.0, EPS);
    EXPECT_NEAR(n.z, 1.0, EPS);
}

TEST(GeometryTest, InfinityCheck) {
    Point2D p{1, 2, 0};

    EXPECT_TRUE(isAtInfinity(p));
}

TEST(GeometryTest, CrossSymmetry) {
    Point2D p1{0, 0, 1};
    Point2D p2{1, 1, 1};

    Line2D l = join(p1, p2);
    Line2D l_rev = join(p2, p1);

    EXPECT_NEAR(l.x, -l_rev.x, EPS);
    EXPECT_NEAR(l.y, -l_rev.y, EPS);
    EXPECT_NEAR(l.z, -l_rev.z, EPS);
}
