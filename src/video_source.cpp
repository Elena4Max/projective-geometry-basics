#include "camera/video_source.hpp"

namespace camera
{

VideoSource::VideoSource(
    const std::filesystem::path& path)
    : source_(path)
{
    capture_.open(path.string());
}

std::optional<Frame>
VideoSource::nextFrame()
{
    Frame frame;

    if (!capture_.read(frame.image))
    {
        return std::nullopt;
    }

    frame.sequence = sequence_++;

    frame.timestamp = 0;

    frame.camera = CameraId::Unknown;

    frame.source = source_;

    return frame;
}

}