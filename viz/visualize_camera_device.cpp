#include <iostream>

#include <opencv2/imgcodecs.hpp>

#include "camera/camera_device.hpp"

int main()
{
    camera::CameraDevice camera(0);

    if (!camera.isOpened())
    {
        std::cout << "Failed to initialize camera\n";
        return -1;
    }


    cv::Mat frame;

    if (!camera.read(frame))
    {
        std::cout << "Read failed\n";
        return -1;
    }

    std::cout << "Request completed\n";

    std::cout << "Camera initialized successfully\n";

    return 0;
}
