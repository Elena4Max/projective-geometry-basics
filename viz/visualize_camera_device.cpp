#include <iostream>

#include <opencv2/imgcodecs.hpp>

#include "camera/camera_device.hpp"
#include <memory>

#include "camera/libcamera_camera.hpp"

int main()
{
    auto backend =
        std::make_unique<camera::LibcameraCamera>(0);

    camera::CameraDevice camera(std::move(backend));

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

    if (!frame.empty())
{
    if (cv::imwrite("frame.png", frame))
        std::cout << "Saved frame.png\n";
    else
        std::cout << "Failed to save frame\n";
}

    camera.close();

    return 0;
}
