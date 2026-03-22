#include <gtest/gtest.h>
#include "../src/geometry.h"

constexpr double EPS = 1e-6;

TEST(GeometryTest, JoinPointsIncidence) {
    Point p1{0, 0, 1};
    Point p2{1, 1, 1};

    Line l = join(p1, p2);

    EXPECT_TRUE(incidence(p1, l));
    EXPECT_TRUE(incidence(p2, l));
}

TEST(GeometryTest, MeetLinesIntersection) {
    Line l1{1, 0, 0}; // x=0
    Line l2{0, 1, 0}; // y=0

    Point p = meet(l1, l2);
    p = normalize(p);

    EXPECT_NEAR(p.x, 0.0, EPS);
    EXPECT_NEAR(p.y, 0.0, EPS);
    EXPECT_NEAR(p.z, 1.0, EPS);
}

TEST(GeometryTest, JoinMeetConsistency) {
    Point p1{0, 0, 1};
    Point p2{1, 1, 1};

    Line l = join(p1, p2);

    Line l2{1, -1, 0}; // x - y = 0

    Point p = meet(l, l2);

    EXPECT_TRUE(incidence(p, l));
    EXPECT_TRUE(incidence(p, l2));
}

TEST(GeometryTest, ParallelLinesGoToInfinity) {
    Line l1{0, 1, -1}; // y = 1
    Line l2{0, 1, -2}; // y = 2

    Point p = meet(l1, l2);

    EXPECT_TRUE(isAtInfinity(p));
}

TEST(GeometryTest, NormalizeFinitePoint) {
    Point p{2, 4, 2};

    Point n = normalize(p);

    EXPECT_NEAR(n.x, 1.0, EPS);
    EXPECT_NEAR(n.y, 2.0, EPS);
    EXPECT_NEAR(n.z, 1.0, EPS);
}

TEST(GeometryTest, InfinityCheck) {
    Point p{1, 2, 0};

    EXPECT_TRUE(isAtInfinity(p));
}

TEST(GeometryTest, CrossSymmetry) {
    Point p1{0, 0, 1};
    Point p2{1, 1, 1};

    Line l = join(p1, p2);
    Line l_rev = join(p2, p1);

    EXPECT_NEAR(l.x, -l_rev.x, EPS);
    EXPECT_NEAR(l.y, -l_rev.y, EPS);
    EXPECT_NEAR(l.z, -l_rev.z, EPS);
}

TEST(HomographyTest, IncidencePreserved) {
    Point p{1, 2, 1};
    Line l{1, -1, 0};

    // ASSERT_TRUE(incidence(p, l));

    // TODO:
    // Point p2 = applyHomography(H, p);
    // Line l2 = transformLine(H, l);

    // EXPECT_TRUE(incidence(p2, l2));
}
