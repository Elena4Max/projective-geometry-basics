#include "camera/camera_device.hpp"

namespace camera
{

CameraDevice::CameraDevice(std::unique_ptr<Camera> camera)
    : camera_(std::move(camera))
{
}

bool CameraDevice::open()
{
    return camera_->open();
}

bool CameraDevice::read(cv::Mat& frame)
{
    return camera_->read(frame);
}

void CameraDevice::close()
{
    camera_->close();
}

}
