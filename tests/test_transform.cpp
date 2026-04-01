#include <gtest/gtest.h>
#include <transform/transform2d.hpp>
#include <geometry/operations.hpp>
#include <core/constants.hpp>

TEST(TransformTest, Translation) {
    geometry::Point2D p{1, 2, 1};

    transform::Transform2D t = transform::Transform2D::translation(3, 4);

    geometry::Point2D p2 = t.apply(p);
    p2 = geometry::normalize(p2);

    EXPECT_NEAR(p2.x, 4.0, core::kEps);
    EXPECT_NEAR(p2.y, 6.0, core::kEps);
}

TEST(TransformTest, Scale) {
    geometry::Point2D p{1, 2, 1};

    transform::Transform2D s = transform::Transform2D::scale(2);

    geometry::Point2D p2 = s.apply(p);
    p2 = geometry::normalize(p2);

    EXPECT_NEAR(p2.x, 2.0, core::kEps);
    EXPECT_NEAR(p2.y, 4.0, core::kEps);
}

TEST(TransformTest, Rotation90Deg) {
    geometry::Point2D p{1, 0, 1};

    transform::Transform2D r = transform::Transform2D::rotation(M_PI / 2);

    geometry::Point2D p2 = r.apply(p);
    p2 = geometry::normalize(p2);

    EXPECT_NEAR(p2.x, 0.0, core::kEps);
    EXPECT_NEAR(p2.y, 1.0, core::kEps);
}

TEST(TransformTest, Composition) {
    geometry::Point2D p{1, 1, 1};

    transform::Transform2D t = transform::Transform2D::translation(1, 0);
    transform::Transform2D s = transform::Transform2D::scale(2);

    geometry::Point2D p2 = s.apply(t.apply(p));
    p2 = geometry::normalize(p2);

    EXPECT_NEAR(p2.x, 4.0, core::kEps);
    EXPECT_NEAR(p2.y, 2.0, core::kEps);
}