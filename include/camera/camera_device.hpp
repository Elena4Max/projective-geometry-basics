#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace camera
{

class CameraDevice
{
public:
    explicit CameraDevice(int cameraId = 0);

    bool open();

    bool read(cv::Mat& frame);

    void close();

private:
    int cameraId_;
    cv::VideoCapture capture_;
};

} // namespace camera
