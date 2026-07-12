#pragma once

#include "algorithms/chessboard_detector.hpp"
#include "camera/frame.hpp"

namespace algorithms {

class FrameSelector {
public:
    explicit FrameSelector(std::size_t frameStep = 30,
                           std::size_t maxFrames = 20);

    bool accept(const camera::Frame&, const ChessboardDetection&) const;

    bool finished() const;

private:
    std::size_t frameStep_;
    std::size_t maxFrames_;

    mutable std::size_t acceptedFrames_ = 0;
};

}  // namespace algorithms