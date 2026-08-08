#pragma once

#include <opencv2/core.hpp>

namespace visualization {

class StreamViewer {
public:
    StreamViewer();

    bool start(std::uint16_t port = 8080);

    void send(const cv::Mat& frame);

    void stop();

private:
    bool running_{false};
};

} // namespace visualization