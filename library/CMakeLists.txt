find_package(Eigen3 REQUIRED)

add_library(cv_geometry
    src/operations.cpp
    src/transform2d.cpp
    src/homography.cpp
    src/homography_estimation.cpp
    src/camera_projection.cpp
    src/rotation.cpp
)

target_include_directories(cv_geometry PUBLIC include)
target_compile_features(cv_geometry PUBLIC cxx_std_20)
target_link_libraries(cv_geometry PUBLIC Eigen3::Eigen)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
