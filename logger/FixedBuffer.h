#ifndef FIXED_BUFFER_H
#define FIXED_BUFFER_H
#include <cstdio>
#include <string>
#include <strings.h>
using std::string;
using std::string_view;

static const int KFIXEDBUFFERSIZE = 4096;
static const int KFIXEDLARGERBUFFERSIZE = 8192;

class FixedBuffer
{
  public:
    FixedBuffer(int capacity = KFIXEDBUFFERSIZE) : capacity_(capacity), data_(new char[capacity_]), cur_(data_){};
    ~FixedBuffer()
    {
        delete[] data_;
        data_ = cur_ = nullptr;
    };

    void append(const char *buf, int len);
    void append(const string &buf);
    void append(char c);

    const char *data() const { return data_; }
    int size() const { return static_cast<int>(cur_ - data_); }

    int avail() const { return data_ + capacity_ - cur_; }
    void reset() { cur_ = data_; }
    void clear() { bzero(data_, capacity_); }

  private:
    const int capacity_;
    char *data_;
    char *cur_ = nullptr;
};

#endif