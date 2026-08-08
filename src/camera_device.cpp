#include "camera/camera_device.hpp"

namespace camera {

CameraDevice::CameraDevice(std::unique_ptr<Camera> camera) : camera(std::move(camera)) {}

bool CameraDevice::open() { return camera->open(); }

std::optional<Frame> CameraDevice::nextFrame() { return camera->nextFrame(); }

void CameraDevice::close() { camera->close(); }

}  // namespace camera
