#include "algorithms/frame_selector.hpp"

namespace algorithms {

FrameSelector::FrameSelector(std::size_t frameStep, std::size_t maxFrames)
    : frameStep(frameStep), maxFrames(maxFrames) {}

bool FrameSelector::accept(const camera::Frame& frame, const ChessboardDetection&) const {
    if (acceptedFrames >= maxFrames) {
        return false;
    }

    if (frame.sequence % frameStep != 0) {
        return false;
    }

    ++acceptedFrames;

    return true;
}

bool FrameSelector::finished() const { return acceptedFrames >= maxFrames; }

}  // namespace algorithms