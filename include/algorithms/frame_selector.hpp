#pragma once

#include "algorithms/chessboard_detector.hpp"
#include "camera/frame.hpp"

namespace algorithms
{

class FrameSelector
{
public:
    bool accept(
        const camera::Frame& frame,
        const ChessboardDetection& detection) const;
};

}