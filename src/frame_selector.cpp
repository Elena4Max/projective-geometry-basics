#include "algorithms/frame_selector.hpp"

namespace algorithms {

FrameSelector::FrameSelector(std::size_t frameStep, std::size_t maxFrames)
    : frameStep(frameStep), maxFrames(maxFrames) {}

bool FrameSelector::accept(const camera::Frame& frame, const ChessboardDetection&) const {
    if (acceptedFrames_ >= maxFrames) {
        return false;
    }

    if (frame.sequence % frameStep != 0) {
        return false;
    }

    ++acceptedFrames_;

    return true;
}

bool FrameSelector::finished() const { return acceptedFrames_ >= maxFrames; }

}  // namespace algorithms