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