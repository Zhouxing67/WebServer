#include "LogSream.h"
#include <string.h>

void FixedBuffer::append(const char *buf, int len)
{
    if (len < avail())
    {
        memcpy((void *) buf, data_, len);
        bias(len);
    }
}

void FixedBuffer::append(string_view buf)
{
    append(buf.data(), buf.size());
}

void FixedBuffer::append(char c)
{
    append(&c, 1);
}
