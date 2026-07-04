#include <iostream>
#include <memory>

#include <opencv2/imgcodecs.hpp>

#include "camera/camera_device.hpp"
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

    auto frame = camera.nextFrame();

    if (!frame)
    {
        std::cout << "Failed to capture frame\n";
        return -1;
    }

    std::cout
        << "Captured frame: "
        << frame->image.cols
        << " x "
        << frame->image.rows
        << '\n';

    if (!frame->image.empty())
    {
        if (cv::imwrite("frame.png", frame->image))
            std::cout << "Saved frame.png\n";
        else
            std::cout << "Failed to save frame\n";
    }

    camera.close();

    return 0;
}