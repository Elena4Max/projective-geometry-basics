#pragma once

#include <opencv2/core.hpp>

#include <vector>

#include "camera/frame.hpp"

namespace algorithms
{

struct ChessboardDetection
{
    bool found = false;

    std::vector<cv::Point2f> corners;
};

class ChessboardDetector
{
public:
    explicit ChessboardDetector(
        cv::Size patternSize);

    ChessboardDetection detect(
        const camera::Frame& frame) const;

private:
    cv::Size patternSize_;
};

}