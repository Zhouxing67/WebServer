#include "Buffer.h"

void Buffer::append(const char *str, int size)
{
    buf_.append(str, size);
}

void Buffer::getline(istream &is)
{
    buf_.clear();
    std::getline(is, buf_);
}

void Buffer::set_buf(const char *_buf)
{
    buf_.clear();
    buf_.append(_buf);
}