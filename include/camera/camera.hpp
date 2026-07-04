#pragma once

#include <optional>
#include "camera/frame.hpp"

namespace camera
{

class Camera
{
public:
    virtual ~Camera() = default;

    virtual bool open() = 0;

    virtual std::optional<Frame> nextFrame() = 0;

    virtual void close() = 0;
};

}