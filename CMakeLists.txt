cmake_minimum_required(VERSION 3.10)

project(ProjectiveGeometry)

include(CTest)

option(BUILD_RASPBERRY_CAMERA "Build Raspberry Pi camera support" OFF)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(geometry
    src/operations.cpp
    src/transform2d.cpp
    src/homography.cpp
    src/homography_estimation.cpp
)

target_include_directories(
    geometry
    PUBLIC include
)

target_compile_features(
    geometry
    PUBLIC cxx_std_20
)

target_link_libraries(
    geometry
    PUBLIC Eigen3::Eigen
)

set(CAMERA_SOURCES
    src/camera_projection.cpp
    src/camera_device.cpp
    src/rotation.cpp
    src/image_directory.cpp
)

if(BUILD_RASPBERRY_CAMERA)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

    list(APPEND CAMERA_SOURCES
        src/libcamera_camera.cpp
        src/mapped_frame.cpp
    )

endif()

add_library(camera ${CAMERA_SOURCES})

target_include_directories(camera
    PUBLIC
        include
)

target_compile_features(
    camera
    PUBLIC cxx_std_20
)

target_link_libraries(
    camera
    PUBLIC
        geometry
        ${OpenCV_LIBS}
)

if(BUILD_RASPBERRY_CAMERA)

    target_link_libraries(
        camera
        PUBLIC
            PkgConfig::LIBCAMERA
    )

    target_compile_definitions(
        camera
        PUBLIC
            HAVE_LIBCAMERA
    )

endif()

add_library(algorithms
    src/chessboard_detector.cpp
)

target_include_directories(
    algorithms
    PUBLIC include
)

target_compile_features(
    algorithms
    PUBLIC cxx_std_20
)

target_link_libraries(
    algorithms
    PUBLIC
        camera
        ${OpenCV_LIBS}
)

add_executable(visualize
    # viz/visualize_geometry.cpp
    # viz/visualize_panorama.cpp
    # viz/visualize_camera.cpp
    # viz/visualize_camera_device.cpp
     viz/visualize_camera_calibration.cpp
)

target_link_libraries(
    visualize
    PRIVATE
        geometry
        camera
        algorithms
        ${OpenCV_LIBS}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()