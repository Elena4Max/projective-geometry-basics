find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

add_executable(lab_camera_projection main.cpp)
target_link_libraries(lab_camera_projection PRIVATE cv_geometry ${OpenCV_LIBS})
