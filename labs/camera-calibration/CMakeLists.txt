find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs videoio calib3d)

add_executable(lab_camera_calibration
    main.cpp
    src/calibration.cpp
    src/calibration_io.cpp
    src/chessboard_detector.cpp
    src/image_directory.cpp
    src/video_source.cpp
)
target_include_directories(lab_camera_calibration PRIVATE include)
target_link_libraries(lab_camera_calibration PRIVATE cv_geometry ${OpenCV_LIBS})

if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    add_executable(calibration_io_tests
        tests/test_calibration_io.cpp
        src/calibration_io.cpp
    )
    target_include_directories(calibration_io_tests PRIVATE include)
    target_link_libraries(calibration_io_tests PRIVATE GTest::gtest_main ${OpenCV_LIBS})
    add_test(NAME calibration_io_tests COMMAND calibration_io_tests)
endif()
