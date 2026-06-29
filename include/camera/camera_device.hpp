#pragma once

#include <memory>

#include "camera/camera.hpp"

namespace camera
{

class CameraDevice
{
public:
    explicit CameraDevice(std::unique_ptr<Camera> camera);

    bool open();

    bool read(cv::Mat& frame);

    void close();

private:
    std::unique_ptr<Camera> camera_;
};

}
