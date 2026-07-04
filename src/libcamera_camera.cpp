#include "camera/libcamera_camera.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace camera {

LibcameraCamera::LibcameraCamera(int cameraId) : cameraId_(cameraId) {}

LibcameraCamera::~LibcameraCamera() { close(); }

bool LibcameraCamera::open() {
    manager_ = std::make_unique<libcamera::CameraManager>();

    if (manager_->start()) return false;

    const auto& cameras = manager_->cameras();

    std::cout << "Found cameras: " << cameras.size() << '\n';

    if (cameraId_ >= static_cast<int>(cameras.size())) return false;

    camera_ = cameras[cameraId_];

    if (!camera_) return false;

    if (camera_->acquire()) return false;

    configuration_ = camera_->generateConfiguration({libcamera::StreamRole::Viewfinder});

    if (!configuration_) return false;

    configuration_->validate();

    if (camera_->configure(configuration_.get())) return false;

    const auto& cfg = configuration_->at(0);

    width_ = cfg.size.width;
    height_ = cfg.size.height;

    std::cout << "Stream configuration:\n"
              << cfg.size.width << " x " << cfg.size.height << '\n'
              << cfg.pixelFormat.toString() << '\n';

    allocator_ = std::make_unique<libcamera::FrameBufferAllocator>(camera_);

    auto* stream = configuration_->at(0).stream();

    if (allocator_->allocate(stream) < 0) return false;

    const auto& buffers = allocator_->buffers(stream);

    std::cout << "Allocated buffers: " << buffers.size() << '\n';

    if (buffers.empty()) return false;

    for (const auto& buffer : buffers) {
        auto request = camera_->createRequest();

        if (!request) return false;

        if (request->addBuffer(stream, buffer.get())) return false;

        requests_.push_back(std::move(request));
    }

    std::cout << "Created requests: " << requests_.size() << '\n';

    if (camera_->start()) return false;

    std::cout << "Camera started\n";

    camera_->requestCompleted.connect(this, &LibcameraCamera::requestCompleted);

    for (auto& request : requests_) {
        if (camera_->queueRequest(request.get())) return false;
    }

    std::cout << "Requests queued\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));

    opened_ = true;

    return true;
}

std::optional<Frame> LibcameraCamera::nextFrame() {
    Frame frame;
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock, [this] { return completedRequest_ != nullptr; });

    frame =
        cv::Mat(static_cast<int>(height_), static_cast<int>(width_), CV_8UC4, mappedFrame_.data());

    completedRequest_ = nullptr;

    return frame;
}

void LibcameraCamera::close() {
    if (camera_) {
        camera_->stop();

        requests_.clear();
        allocator_.reset();

        camera_->release();
        camera_.reset();
    }

    if (manager_) {
        manager_->stop();
        manager_.reset();
    }

    opened_ = false;
}

void LibcameraCamera::requestCompleted(libcamera::Request* request) {
    if (request->status() == libcamera::Request::RequestCancelled) return;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        completedRequest_ = request;
    }

    const auto& buffer = request->buffers().begin()->second;

    const auto& plane = buffer->planes()[0];

    if (!mappedFrame_.data()) {
        if (!mappedFrame_.map(plane.fd.get(), plane.length)) {
            std::cerr << "mmap failed\n";
            return;
        }

        std::cout << "Mapped " << mappedFrame_.size() << " bytes\n";
    }

    request->reuse(libcamera::Request::ReuseBuffers);

    if (camera_->queueRequest(request)) return;

    condition_.notify_one();
}
}  // namespace camera
