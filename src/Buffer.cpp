#include "Buffer.h"

void Buffer::append(const char *str, int size)
{
    buf_.append(str, size);
}