#pragma once

#include <memory>

#include <opencv2/core.hpp>

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/framebuffer_allocator.h>
#include <libcamera/request.h>

#include "camera/mapped_frame.hpp"
#include "camera/camera.hpp"

#include <condition_variable>
#include <mutex>

namespace camera
{

class LibcameraCamera final : public Camera
{
public:
    explicit LibcameraCamera(int cameraId = 0);
    ~LibcameraCamera() override;

    bool open() override;
    bool read(cv::Mat& frame) override;
    void close() override;

private:
    int cameraId_;

    bool opened_{false};

    std::unique_ptr<libcamera::CameraManager> manager_;
    std::shared_ptr<libcamera::Camera> camera_;
    std::unique_ptr<libcamera::CameraConfiguration> configuration_;
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator_;
    std::vector<std::unique_ptr<libcamera::Request>> requests_;

void requestCompleted(libcamera::Request* request);

std::mutex mutex_;
std::condition_variable condition_;

libcamera::Request* completedRequest_{nullptr};
MappedFrame mappedFrame_;


std::uint32_t width_{0};
std::uint32_t height_{0};

};

}
