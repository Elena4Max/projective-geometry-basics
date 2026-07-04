#include "camera/image_directory.hpp"

#include <opencv2/imgcodecs.hpp>

#include <algorithm>

namespace fs = std::filesystem;

namespace camera
{

ImageDirectory::ImageDirectory(
    const fs::path& directory)
{
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (!entry.is_regular_file())
            continue;

        imagePaths_.push_back(entry.path());
    }

    std::sort(
        imagePaths_.begin(),
        imagePaths_.end());
}

std::optional<Frame>
ImageDirectory::nextFrame()
{
    while (currentIndex_ < imagePaths_.size())
    {
        const auto& path =
            imagePaths_[currentIndex_++];

        cv::Mat image =
            cv::imread(path.string());

        if (image.empty())
            continue;

        Frame frame;

        frame.image = std::move(image);
        frame.source = path;
        frame.sequence = sequence_++;

        return frame;
    }

    return std::nullopt;
}

}