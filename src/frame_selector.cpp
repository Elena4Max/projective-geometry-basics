#include "algorithms/frame_selector.hpp"

namespace algorithms {

FrameSelector::FrameSelector(std::size_t frameStep,
                             std::size_t maxFrames)
    : frameStep_(frameStep),
      maxFrames_(maxFrames) {}

bool FrameSelector::accept(const camera::Frame& frame,
                           const ChessboardDetection&) const
{
    if (acceptedFrames_ >= maxFrames_) {
        return false;
    }

    if (frame.sequence % frameStep_ != 0) {
        return false;
    }

    ++acceptedFrames_;

    return true;
}

bool FrameSelector::finished() const
{
    return acceptedFrames_ >= maxFrames_;
}

}  // namespace algorithms