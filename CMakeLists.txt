cmake_minimum_required(VERSION 3.10)
project(ProjectiveGeometry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(geometry
    src/operations.cpp
    src/transform2d.cpp
    src/homography.cpp
)

find_package(OpenCV REQUIRED)

add_executable(visualize
    viz/visualize.cpp
)

target_link_libraries(visualize
    geometry
    ${OpenCV_LIBS}
)

find_package(GTest REQUIRED)

add_executable(tests
    tests/test_geometry.cpp
    tests/test_transform.cpp
    tests/test_homography.cpp
)

target_link_libraries(tests
    geometry
    GTest::gtest
    GTest::gtest_main
    pthread
)