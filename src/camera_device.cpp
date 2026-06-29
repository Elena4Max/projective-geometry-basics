#include "camera/camera_device.hpp"

namespace camera
{

CameraDevice::CameraDevice(int cameraId)
    : cameraId_(cameraId)
{
}

bool CameraDevice::open()
{
    return capture_.open(cameraId_);
}

bool CameraDevice::read(cv::Mat& frame)
{
    return capture_.read(frame);
}

void CameraDevice::close()
{
    capture_.release();
}

}
