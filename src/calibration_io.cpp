#include "camera/calibration_io.hpp"

#include <opencv2/core.hpp>

namespace camera {

bool CalibrationIO::save(const std::filesystem::path& filename,
                         const cv::Mat& cameraMatrix,
                         const cv::Mat& distortion,
                         const cv::Size& imageSize,
                         double rms)
{
    cv::FileStorage fs(filename.string(), cv::FileStorage::WRITE);

    if (!fs.isOpened())
        return false;

    fs << "cameramatrix" << cameraMatrix;
    fs << "distortion_coefficients" << distortion;
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "rms" << rms;

    return true;
}

bool CalibrationIO::load(const std::filesystem::path& filename,
                         cv::Mat& cameraMatrix,
                         cv::Mat& distortion,
                         cv::Size& imageSize,
                         double& rms)
{
    cv::FileStorage fs(filename.string(), cv::FileStorage::READ);

    if (!fs.isOpened())
        return false;

    fs["cameramatrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distortion;

    fs["image_width"] >> imageSize.width;
    fs["image_height"] >> imageSize.height;

    fs["rms"] >> rms;

    return true;
}

} // namespace camera