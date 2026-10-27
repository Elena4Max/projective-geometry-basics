find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs features2d calib3d)

add_executable(lab_panorama_stitching main.cpp)
target_link_libraries(lab_panorama_stitching PRIVATE cv_geometry ${OpenCV_LIBS})
