#include "algorithms/frame_selector.hpp"

namespace algorithms {

bool FrameSelector::accept(const camera::Frame&, const ChessboardDetection&) const { return true; }

}  // namespace algorithms