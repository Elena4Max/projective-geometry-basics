#include "camera/libcamera_camera.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace camera {

LibcameraCamera::LibcameraCamera(int cameraId) : cameraId_(cameraId) {}

LibcameraCamera::~LibcameraCamera() { close(); }

bool LibcameraCamera::open() {
    if (!initializeManager()) {
        return false;
    }

    if (!acquireCamera()) {
        return false;
    }

    if (!configureStream()) {
        return false;
    }

    if (!allocateBuffers()) {
        return false;
    }

    if (!createRequests()) {
        return false;
    }

    if (!startCamera()) {
        return false;
    }

    if (!queueRequests()) {
        return false;
    }

    opened_ = true;

    return true;
}

bool LibcameraCamera::initializeManager() {
    manager_ = std::make_unique<libcamera::CameraManager>();

    if (manager_->start()) {
        return false;
    }

    return true;
}

bool LibcameraCamera::acquireCamera() {
    const auto& cameras = manager_->cameras();

    std::cout << "Found cameras: " << cameras.size() << '\n';

    if (cameraId_ < 0 || static_cast<std::size_t>(cameraId_) >= cameras.size()) {
        return false;
    }

    camera_ = cameras[cameraId_];

    if (!camera_) {
        return false;
    }

    if (camera_->acquire()) {
        return false;
    }

    return true;
}

bool LibcameraCamera::configureStream() {
    configuration_ = camera_->generateConfiguration({libcamera::StreamRole::Viewfinder});

    if (!configuration_) {
        return false;
    }

    configuration_->validate();

    if (camera_->configure(configuration_.get())) {
        return false;
    }

    const auto& cfg = configuration_->at(0);

    stream_ = cfg.stream();

    width_ = cfg.size.width;
    height_ = cfg.size.height;

    std::cout << "Stream configuration:\n"
              << width_ << " x " << height_ << '\n'
              << cfg.pixelFormat.toString() << '\n';

    return true;
}

bool LibcameraCamera::allocateBuffers() {
    allocator_ = std::make_unique<libcamera::FrameBufferAllocator>(camera_);

    if (allocator_->allocate(stream_) < 0) {
        return false;
    }

    const auto& buffers = allocator_->buffers(stream_);

    std::cout << "Allocated buffers: " << buffers.size() << '\n';

    if (buffers.empty()) {
        return false;
    }

    return true;
}

bool LibcameraCamera::createRequests() {
    const auto& buffers = allocator_->buffers(stream_);

    requests_.clear();
    requests_.reserve(buffers.size());

    for (const auto& buffer : buffers) {
        auto request = camera_->createRequest();

        if (!request) {
            return false;
        }

        if (request->addBuffer(stream_, buffer.get())) {
            return false;
        }

        requests_.push_back(std::move(request));
    }

    std::cout << "Created requests: " << requests_.size() << '\n';

    return true;
}

bool LibcameraCamera::startCamera() {
    camera_->requestCompleted.connect(this, &LibcameraCamera::requestCompleted);

    if (camera_->start()) {
        return false;
    }

    std::cout << "Camera started\n";

    return true;
}

bool LibcameraCamera::queueRequests() {
    for (auto& request : requests_) {
        if (camera_->queueRequest(request.get())) {
            return false;
        }
    }

    std::cout << "Requests queued\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return true;
}

std::optional<Frame> LibcameraCamera::nextFrame() {
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock, [this] { return completedRequest_ != nullptr; });

    Frame frame;

    frame.image =
        cv::Mat(static_cast<int>(height_), static_cast<int>(width_), CV_8UC4, mappedFrame_.data());

    frame.camera = cameraId_ == 0 ? CameraId::Left : CameraId::Right;

    frame.timestamp = 0;
    frame.sequence = 0;

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
