#include <gtest/gtest.h>

#include <camera/camera_extrinsics.hpp>
#include <camera/projection.hpp>
#include <camera/rotation.hpp>

using camera::CameraIntrinsics;

TEST(CameraProjection, CenterProjection) {
    CameraIntrinsics K{500.0, 500.0, 400.0, 300.0};

    core::Vec3 X{0.0, 0.0, 2.0};

    auto p = camera::projectPoint(X, K);

    EXPECT_NEAR(p.x, 400.0, 1e-9);
    EXPECT_NEAR(p.y, 300.0, 1e-9);
}

TEST(CameraProjection, PerspectiveDivision) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    core::Vec3 X{2.0, 4.0, 2.0};

    auto p = camera::projectPoint(X, K);

    EXPECT_NEAR(p.x, 500.0, 1e-9);
    EXPECT_NEAR(p.y, 1000.0, 1e-9);
}

TEST(CameraProjection, FartherPointsAppearSmaller) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    core::Vec3 nearPoint{2.0, 0.0, 2.0};

    core::Vec3 farPoint{2.0, 0.0, 4.0};

    auto pNear = camera::projectPoint(nearPoint, K);
    auto pFar = camera::projectPoint(farPoint, K);

    EXPECT_GT(pNear.x, pFar.x);
}

TEST(CameraProjection, TranslationChangesProjection) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    camera::CameraExtrinsics E{core::Mat3::identity(), {1.0, 0.0, 0.0}};

    core::Vec3 X{0.0, 0.0, 2.0};

    auto p = camera::projectPoint(X, K, E);

    EXPECT_NEAR(p.x, 250.0, 1e-9);
    EXPECT_NEAR(p.y, 0.0, 1e-9);
}

TEST(CameraProjection, RotationXTiltsPointDown) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    camera::CameraExtrinsics E{camera::rotationX(0.5), {0.0, 0.0, 0.0}};

    core::Vec3 X{0.0, 1.0, 3.0};

    auto p1 = camera::projectPoint(X, K);

    auto p2 = camera::projectPoint(X, K, E);

    EXPECT_NEAR(p1.x, p2.x, 1e-9);

    EXPECT_LT(p2.y, p1.y);
}

TEST(CameraProjection, RotationYMovesPointSideways) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    camera::CameraExtrinsics E{camera::rotationY(0.5), {0.0, 0.0, 0.0}};

    core::Vec3 X{1.0, 0.0, 3.0};

    auto p1 = camera::projectPoint(X, K);

    auto p2 = camera::projectPoint(X, K, E);

    EXPECT_NEAR(p1.y, p2.y, 1e-9);

    EXPECT_GT(p2.x, p1.x);
}

TEST(CameraProjection, RotationZRotatesImagePoint) {
    CameraIntrinsics K{500.0, 500.0, 0.0, 0.0};

    camera::CameraExtrinsics E{camera::rotationZ(0.5), {0.0, 0.0, 0.0}};

    core::Vec3 X{1.0, 0.0, 3.0};

    auto p1 = camera::projectPoint(X, K);

    auto p2 = camera::projectPoint(X, K, E);

    EXPECT_GT(p2.y, 0.0);

    EXPECT_LT(p2.x, p1.x);
}