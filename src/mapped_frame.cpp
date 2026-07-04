#include "camera/mapped_frame.hpp"

#include <sys/mman.h>

namespace camera {

MappedFrame::~MappedFrame() { unmap(); }

MappedFrame::MappedFrame(MappedFrame&& other) noexcept {
    data_ = other.data_;
    size_ = other.size_;

    other.data_ = nullptr;
    other.size_ = 0;
}

MappedFrame& MappedFrame::operator=(MappedFrame&& other) noexcept {
    if (this == &other) return *this;

    unmap();

    data_ = other.data_;
    size_ = other.size_;

    other.data_ = nullptr;
    other.size_ = 0;

    return *this;
}

bool MappedFrame::map(int fd, std::size_t length) {
    unmap();

    void* memory = mmap(nullptr, length, PROT_READ, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED) return false;

    data_ = static_cast<unsigned char*>(memory);
    size_ = length;

    return true;
}

void MappedFrame::unmap() {
    if (!data_) return;

    munmap(data_, size_);

    data_ = nullptr;
    size_ = 0;
}

unsigned char* MappedFrame::data() const noexcept { return data_; }

std::size_t MappedFrame::size() const noexcept { return size_; }

}  // namespace camera
