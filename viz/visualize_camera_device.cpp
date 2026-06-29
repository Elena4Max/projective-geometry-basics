#include <iostream>

#include <opencv2/imgcodecs.hpp>

#include "camera/camera_device.hpp"

int main()
{
    camera::CameraDevice camera(0);

    if (!camera.open())
    {
        std::cout << "Failed to open camera\n";
        return -1;
    }

    cv::Mat frame;

    if (!camera.read(frame))
    {
        std::cout << "Failed to capture frame\n";
        return -1;
    }

    std::cout << "Captured frame: "
              << frame.cols << " x "
              << frame.rows << '\n';

    cv::imwrite("frame.png", frame);

    camera.close();

    return 0;
}
