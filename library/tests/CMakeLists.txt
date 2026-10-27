find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(geometry_tests
    test_geometry.cpp
    test_transform.cpp
    test_homography.cpp
    test_homography_estimation.cpp
)
target_link_libraries(geometry_tests PRIVATE cv_geometry GTest::gtest_main Threads::Threads)
target_compile_features(geometry_tests PRIVATE cxx_std_20)
add_test(NAME geometry_tests COMMAND geometry_tests)

add_executable(camera_projection_tests test_camera_projection.cpp)
target_link_libraries(camera_projection_tests PRIVATE cv_geometry GTest::gtest_main)
target_compile_features(camera_projection_tests PRIVATE cxx_std_20)
add_test(NAME camera_projection_tests COMMAND camera_projection_tests)
