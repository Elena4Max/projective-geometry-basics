#pragma once

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/framebuffer_allocator.h>
#include <libcamera/request.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <opencv2/core.hpp>

#include "camera/camera.hpp"
#include "camera/mapped_frame.hpp"

namespace camera {

class LibcameraCamera final : public Camera {
   public:
    explicit LibcameraCamera(int cameraId = 0);
    ~LibcameraCamera() override;

    bool open() override;
    std::optional<Frame> nextFrame() override;
    void close() override;

   private:
    bool initializeManager();
    bool acquireCamera();
    bool configureStream();
    bool allocateBuffers();
    bool createRequests();
    bool startCamera();
    bool queueRequests();

    libcamera::Stream* stream_ = nullptr;

    int cameraId_;

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

}  // namespace camera
