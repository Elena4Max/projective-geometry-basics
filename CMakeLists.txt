cmake_minimum_required(VERSION 3.10)
project(ProjectiveGeometry)

include(CTest)

add_library(geometry
    src/operations.cpp
    src/transform2d.cpp
    src/homography.cpp
    src/homography_estimation.cpp
)

target_include_directories(geometry PUBLIC include)

target_compile_features(geometry PUBLIC cxx_std_20)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(visualize
    #viz/visualize_geometry.cpp
    viz/visualize_panorama.cpp
)

target_link_libraries(geometry
    PUBLIC Eigen3::Eigen
)

target_link_libraries(visualize
    geometry
    ${OpenCV_LIBS}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()