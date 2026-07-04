#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include "camera/frame.hpp"

namespace camera {

class ImageDirectory {
   public:
    explicit ImageDirectory(const std::filesystem::path& directory);

    std::optional<Frame> nextFrame();

   private:
    std::vector<std::filesystem::path> imagePaths_;

    std::size_t currentIndex_{0};

    uint64_t sequence_{0};
};

}  // namespace camera