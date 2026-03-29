#include <gtest/gtest.h>
#include "../include/transform/transform2d.h"
#include "../include/geometry/operations.h"

constexpr double EPS = 1e-6;

TEST(TransformTest, Translation) {
    Point2D p{1, 2, 1};

    Transform2D t = Transform2D::translation(3, 4);

    Point2D p2 = t.apply(p);
    p2 = normalize(p2);

    EXPECT_NEAR(p2.x, 4.0, EPS);
    EXPECT_NEAR(p2.y, 6.0, EPS);
}

TEST(TransformTest, Scale) {
    Point2D p{1, 2, 1};

    Transform2D s = Transform2D::scale(2);

    Point2D p2 = s.apply(p);
    p2 = normalize(p2);

    EXPECT_NEAR(p2.x, 2.0, EPS);
    EXPECT_NEAR(p2.y, 4.0, EPS);
}

TEST(TransformTest, Rotation90Deg) {
    Point2D p{1, 0, 1};

    Transform2D r = Transform2D::rotation(M_PI / 2);

    Point2D p2 = r.apply(p);
    p2 = normalize(p2);

    EXPECT_NEAR(p2.x, 0.0, EPS);
    EXPECT_NEAR(p2.y, 1.0, EPS);
}

TEST(TransformTest, Composition) {
    Point2D p{1, 1, 1};

    Transform2D t = Transform2D::translation(1, 0);
    Transform2D s = Transform2D::scale(2);

    Point2D p2 = s.apply(t.apply(p));
    p2 = normalize(p2);

    EXPECT_NEAR(p2.x, 4.0, EPS);
    EXPECT_NEAR(p2.y, 2.0, EPS);
}