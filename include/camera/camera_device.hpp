#pragma once

#include <memory>

#include <opencv2/core.hpp>

namespace libcamera
{
class Camera;
class CameraManager;
class FrameBufferAllocator;
}

namespace camera
{

class CameraDevice
{
public:
    explicit CameraDevice(int cameraId = 0);
    ~CameraDevice();

    CameraDevice(const CameraDevice&) = delete;
    CameraDevice& operator=(const CameraDevice&) = delete;

    CameraDevice(CameraDevice&&) = delete;
    CameraDevice& operator=(CameraDevice&&) = delete;

    [[nodiscard]]
    bool isOpened() const noexcept;

    bool read(cv::Mat& frame);

private:
    bool open();
    void close() noexcept;

private:
    int cameraId_;

    bool opened_{false};

    std::unique_ptr<libcamera::CameraManager> manager_;
    std::shared_ptr<libcamera::Camera> camera_;
    std::unique_ptr<libcamera::FrameBufferAllocator> allocator_;
};

} // namespace camera
