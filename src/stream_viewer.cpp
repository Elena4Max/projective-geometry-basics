#include "visualization/stream_viewer.hpp"

namespace visualization {

StreamViewer::StreamViewer() = default;

bool StreamViewer::start(std::uint16_t) {
    running = true;
    return true;
}

void StreamViewer::send(const cv::Mat&) {
    if (!running) {
        return;
    }

    // TODO:
    // MJPEG streaming
}

void StreamViewer::stop() { running = false; }

}  // namespace visualization