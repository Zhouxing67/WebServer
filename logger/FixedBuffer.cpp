#include "FixedBuffer.h"
#include <string.h>

void FixedBuffer::append(const char *buf, int len)
{
    if (len < avail())
    {
        memcpy(cur_, buf, len);
        bias(len);
    }
}

void FixedBuffer::append(const std::string& buf)
{
    append(buf.c_str(), buf.size());
}

void FixedBuffer::append(char c)
{
    append(&c, 1);
}
