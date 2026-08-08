#include "camera/libcamera_camera.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace camera {

LibcameraCamera::LibcameraCamera(int cameraId) : cameraId(cameraId) {}

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

    if (cameraId < 0 || static_cast<std::size_t>(cameraId) >= cameras.size()) {
        return false;
    }

    camera = cameras[cameraId];

    if (!camera) {
        return false;
    }

    if (camera->acquire()) {
        return false;
    }

    return true;
}

bool LibcameraCamera::configureStream() {
    configuration = camera->generateConfiguration({libcamera::StreamRole::Viewfinder});

    if (!configuration) {
        return false;
    }

    configuration->validate();

    if (camera->configure(configuration.get())) {
        return false;
    }

    const auto& cfg = configuration->at(0);

    stream_ = cfg.stream();

    width = cfg.size.width;
    height = cfg.size.height;

    std::cout << "Stream configuration:\n"
              << width << " x " << height << '\n'
              << cfg.pixelFormat.toString() << '\n';

    return true;
}

bool LibcameraCamera::allocateBuffers() {
    allocator = std::make_unique<libcamera::FrameBufferAllocator>(camera);

    if (allocator->allocate(stream_) < 0) {
        return false;
    }

    const auto& buffers = allocator->buffers(stream_);

    std::cout << "Allocated buffers: " << buffers.size() << '\n';

    if (buffers.empty()) {
        return false;
    }

    return true;
}

bool LibcameraCamera::createRequests() {
    const auto& buffers = allocator->buffers(stream_);

    requests.clear();
    requests.reserve(buffers.size());

    for (const auto& buffer : buffers) {
        auto request = camera->createRequest();

        if (!request) {
            return false;
        }

        if (request->addBuffer(stream_, buffer.get())) {
            return false;
        }

        requests.push_back(std::move(request));
    }

    std::cout << "Created requests: " << requests.size() << '\n';

    return true;
}

bool LibcameraCamera::startCamera() {
    camera->requestCompleted.connect(this, &LibcameraCamera::requestCompleted);

    if (camera->start()) {
        return false;
    }

    std::cout << "Camera started\n";

    return true;
}

bool LibcameraCamera::queueRequests() {
    for (auto& request : requests) {
        if (camera->queueRequest(request.get())) {
            return false;
        }
    }

    std::cout << "Requests queued\n";

    return true;
}

std::optional<Frame> LibcameraCamera::nextFrame() {
    std::unique_lock<std::mutex> lock(mutex);

    condition.wait(lock, [this] { return completedRequest != nullptr; });

    Frame frame;

    frame.image =
        cv::Mat(static_cast<int>(height), static_cast<int>(width), CV_8UC4, mappedFrame.data());

    frame.camera = cameraId == 0 ? CameraId::Left : CameraId::Right;

    frame.timestamp = 0;
    frame.sequence = 0;

    completedRequest = nullptr;

    return frame;
}

void LibcameraCamera::close() {
    if (camera) {
        camera->stop();

        requests.clear();
        allocator.reset();

        camera->release();
        camera.reset();
    }

    if (manager_) {
        manager_->stop();
        manager_.reset();
    }
}

void LibcameraCamera::requestCompleted(libcamera::Request* request) {
    if (request->status() == libcamera::Request::RequestCancelled) return;

    {
        std::lock_guard<std::mutex> lock(mutex);
        completedRequest = request;
    }

    const auto& buffer = request->buffers().begin()->second;

    const auto& plane = buffer->planes()[0];

    if (!mappedFrame.data()) {
        if (!mappedFrame.map(plane.fd.get(), plane.length)) {
            std::cerr << "mmap failed\n";
            return;
        }

        std::cout << "Mapped " << mappedFrame.size() << " bytes\n";
    }

    request->reuse(libcamera::Request::ReuseBuffers);

    if (camera->queueRequest(request)) return;

    condition.notify_one();
}
}  // namespace camera
