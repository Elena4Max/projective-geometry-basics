#include "algorithms/chessboard_detector.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace algorithms {

ChessboardDetector::ChessboardDetector(cv::Size patternSize) : patternSize_(patternSize) {}

ChessboardDetection ChessboardDetector::detect(const camera::Frame& frame) const {
    cv::Mat gray;

    cv::cvtColor(frame.image, gray, cv::COLOR_BGR2GRAY);

    ChessboardDetection detection;

    detection.found = cv::findChessboardCornersSB(gray, patternSize_, detection.corners);

    return detection;
}

}  // namespace algorithms