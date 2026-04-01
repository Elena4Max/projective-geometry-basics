cmake_minimum_required(VERSION 3.10)
project(ProjectiveGeometry)

include(CTest)

add_library(geometry
    src/operations.cpp
    src/transform2d.cpp
    src/homography.cpp
)

target_include_directories(geometry PUBLIC include)

target_compile_features(geometry PUBLIC cxx_std_17)

find_package(OpenCV REQUIRED)

add_executable(visualize
    viz/visualize.cpp
)

target_link_libraries(visualize
    geometry
    ${OpenCV_LIBS}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()