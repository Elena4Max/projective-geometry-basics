cmake_minimum_required(VERSION 3.10)
project(ProjectiveGeometry)

set(CMAKE_CXX_STANDARD 17)

# OpenCV
find_package(OpenCV REQUIRED)

# GoogleTest
enable_testing()
find_package(GTest REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(geometry
    src/geometry.cpp
)

# Visualization
add_executable(visualize viz/visualize.cpp)
target_link_libraries(visualize geometry ${OpenCV_LIBS})

# Tests
add_executable(tests tests/test_geometry.cpp)
target_link_libraries(tests geometry GTest::gtest GTest::gtest_main pthread)

add_test(NAME GeometryTests COMMAND tests)
