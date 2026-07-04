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

std::optional<Frame>
CameraDevice::nextFrame()
{
    return camera_->nextFrame();
}

void CameraDevice::close()
{
    camera_->close();
}

}
