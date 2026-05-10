find_package(GTest REQUIRED)

add_executable(geometry_tests
    test_geometry.cpp
    test_transform.cpp
    test_homography.cpp
    test_homography_estimation.cpp
)

target_link_libraries(geometry_tests
    PRIVATE
        geometry
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
)

target_compile_features(geometry_tests PRIVATE cxx_std_20)

add_test(NAME geometry_tests COMMAND geometry_tests)

add_executable(camera_projection_test
    test_camera_projection.cpp
)

target_link_libraries(camera_projection_test
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    geometry
    camera
    Threads::Threads
)

add_test(
    NAME camera_projection_test
    COMMAND camera_projection_test
)

target_compile_features(camera_projection_test PRIVATE cxx_std_20)


