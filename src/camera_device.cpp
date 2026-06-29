#include <libcamera/framebuffer_allocator.h>
#include "camera/camera_device.hpp"

#include <libcamera/libcamera.h>
#include <libcamera/request.h>

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
    static bool first = true;

    if (first)
    {
        for (auto &request : requests_)
        {
            if (camera_->queueRequest(request.get()))
                return false;
        }

        first = false;
    }

    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock, [this]
    {
        return completedRequest_ != nullptr;
    });

    auto *request = completedRequest_;

    completedRequest_ = nullptr;

    lock.unlock();

    request->reuse(libcamera::Request::ReuseBuffers);

    if (camera_->queueRequest(request))
        return false;

    return true;
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

    for (const auto &buffer : buffers)
    {
        auto request = camera_->createRequest();

        if (!request)
            return false;

        if (request->addBuffer(stream, buffer.get()))
            return false;

        requests_.push_back(std::move(request));
    }

    camera_->requestCompleted.connect(
        this,
        &CameraDevice::requestCompleted);

    if (camera_->start())
        return false;

 //   for (auto &request : requests_)
   // {
     //   if (camera_->queueRequest(request.get()))
       //     return false;
    //}


    return true;
}

void CameraDevice::close() noexcept
{
    requests_.clear();

    allocator_.reset();

    if (camera_)
    {
        camera_->requestCompleted.disconnect(
            this,
            &CameraDevice::requestCompleted);

        camera_->stop();
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


void CameraDevice::requestCompleted(libcamera::Request *request)
{
    if (request->status() == libcamera::Request::RequestCancelled)
        return;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        completedRequest_ = request;
    }

    condition_.notify_one();
}
} // namespace camera

