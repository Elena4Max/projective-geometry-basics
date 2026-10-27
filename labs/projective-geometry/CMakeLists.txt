find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

add_executable(lab_projective_geometry main.cpp)
target_link_libraries(lab_projective_geometry PRIVATE cv_geometry ${OpenCV_LIBS})
