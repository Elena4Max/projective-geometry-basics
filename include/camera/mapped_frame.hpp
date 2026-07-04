#pragma once

#include <cstddef>

namespace camera {

class MappedFrame {
   public:
    MappedFrame() = default;

    ~MappedFrame();

    MappedFrame(const MappedFrame&) = delete;
    MappedFrame& operator=(const MappedFrame&) = delete;

    MappedFrame(MappedFrame&&) noexcept;
    MappedFrame& operator=(MappedFrame&&) noexcept;

    bool map(int fd, std::size_t length);

    void unmap();

    [[nodiscard]] unsigned char* data() const noexcept;

    [[nodiscard]] std::size_t size() const noexcept;

   private:
    unsigned char* data_{nullptr};

    std::size_t size_{0};
};

}  // namespace camera
