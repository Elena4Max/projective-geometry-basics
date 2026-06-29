#include <libcamera/framebuffer_allocator.h>
#include "camera/camera_device.hpp"

#include <libcamera/libcamera.h>

namespace camera
{

CameraDevice::CameraDevice(int cameraId)
    : cameraId_(cameraId)
{
    opened_ = open();
}

CameraDevice::~CameraDevice()
{
    close();
}

bool CameraDevice::isOpened() const noexcept
{
    return opened_;
}

bool CameraDevice::read(cv::Mat&)
{
    // Sprint 2.3
    return false;
}

bool CameraDevice::open()
{
    manager_ = std::make_unique<libcamera::CameraManager>();

    if (manager_->start())
        return false;

    const auto& cameras = manager_->cameras();

    if (cameraId_ >= static_cast<int>(cameras.size()))
        return false;

    camera_ = cameras[cameraId_];

    if (!camera_)
        return false;

    if (camera_->acquire())
        return false;

    auto configuration = camera_->generateConfiguration(
    {
        libcamera::StreamRole::Viewfinder
    });

    if (!configuration)
        return false;

    configuration->validate();

    if (camera_->configure(configuration.get()))
        return false;

    allocator_ = std::make_unique<libcamera::FrameBufferAllocator>(camera_);

    auto *stream = configuration->at(0).stream();

    if (allocator_->allocate(stream) < 0)
        return false;

    const auto &buffers = allocator_->buffers(stream);

    if (buffers.empty())
        return false;
    return true;
}

void CameraDevice::close() noexcept
{
    allocator_.reset();

    if (camera_)
    {
        camera_->release();
        camera_.reset();
    }

    if (manager_)
    {
        manager_->stop();
        manager_.reset();
    }

    opened_ = false;
}

} // namespace camera

