#pragma once

#include <opencv2/core.hpp>

namespace camera
{

class Camera
{
public:
    virtual ~Camera() = default;

    virtual bool open() = 0;

    virtual bool read(cv::Mat& frame) = 0;

    virtual void close() = 0;
};

}
