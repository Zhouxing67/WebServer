#include "FixedBuffer.h"
#include <stdexcept>
#include <string.h>

FixedBuffer::FixedBuffer(const FixedBuffer &other)
    : capacity_(other.capacity_), data_(new char[capacity_]), cur_(data_ + other.size())
{
    memcpy(data_, other.data_, capacity_);
}

FixedBuffer &FixedBuffer::operator=(const FixedBuffer &other)
{
    if (this != &other) {
        if (other.capacity_ != capacity_) {
            throw std::runtime_error("operator= failed, the capacity is in-equal");
        }
        memcpy(data_, other.data_, capacity_);
        cur_ = data_ + other.size();
    }
    return *this;
}

FixedBuffer::FixedBuffer(FixedBuffer &&other) noexcept : capacity_(other.capacity_)
{
    move_help(other);
}
FixedBuffer &FixedBuffer::operator=(FixedBuffer &&other) noexcept
{
    if (this != &other && other.capacity_ == capacity_) {
       move_help(other);
    }
    return *this;
}

void FixedBuffer::append(const char *buf, int len)
{
    if (data_ != nullptr && len < avail()) {
        memcpy(cur_, buf, len);
        cur_ += len;
    }
}

void FixedBuffer::append(const std::string &buf) { append(buf.c_str(), buf.size()); }

void FixedBuffer::append(char c) { append(&c, 1); }
