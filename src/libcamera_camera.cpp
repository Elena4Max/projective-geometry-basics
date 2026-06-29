#include "camera/libcamera_camera.hpp"

#include <iostream>

namespace camera
{

LibcameraCamera::LibcameraCamera(int cameraId)
    : cameraId_(cameraId)
{
}

LibcameraCamera::~LibcameraCamera()
{
    close();
}

bool LibcameraCamera::open()
{
    manager_ = std::make_unique<libcamera::CameraManager>();

    if (manager_->start())
        return false;

    const auto& cameras = manager_->cameras();

    std::cout << "Found cameras: "
              << cameras.size()
              << '\n';

    if (cameraId_ >= static_cast<int>(cameras.size()))
        return false;

    camera_ = cameras[cameraId_];

    if (!camera_)
        return false;

    if (camera_->acquire())
        return false;

   configuration_ =
    camera_->generateConfiguration(
    {
        libcamera::StreamRole::Viewfinder
    });

if (!configuration_)
    return false;

configuration_->validate();

if (camera_->configure(configuration_.get()))
    return false;



allocator_ = std::make_unique<libcamera::FrameBufferAllocator>(camera_);

auto* stream = configuration_->at(0).stream();

if (allocator_->allocate(stream) < 0)
    return false;

const auto& buffers = allocator_->buffers(stream);

std::cout
    << "Allocated buffers: "
    << buffers.size()
    << '\n';

if (buffers.empty())
    return false;



for (const auto& buffer : buffers)
{
    auto request = camera_->createRequest();

    if (!request)
        return false;

    if (request->addBuffer(stream, buffer.get()))
        return false;

    requests_.push_back(std::move(request));
}

std::cout
    << "Created requests: "
    << requests_.size()
    << '\n';



const auto& cfg = configuration_->at(0);

std::cout
    << "Stream configuration:\n"
    << cfg.size.width
    << " x "
    << cfg.size.height
    << '\n'
    << cfg.pixelFormat.toString()
    << '\n';



    opened_ = true;

    return true;
}

bool LibcameraCamera::read(cv::Mat&)
{
    return opened_;
}

void LibcameraCamera::close()
{
    if (camera_)
    {
        requests_.clear();
        allocator_.reset();

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

}
