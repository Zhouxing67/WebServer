#ifndef FIXED_BUFFER_H
#define FIXED_BUFFER_H
#include <cstdio>
#include <string>
#include <strings.h>
using std::string;
using std::string_view;
static const int KFIXEDBUFFERSIZE = 4096;

class FixedBuffer
{
  public:
    FixedBuffer() : data_(new char[KFIXEDBUFFERSIZE]), cur_(data_){};
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

    int avail() const { return data_ + KFIXEDBUFFERSIZE - cur_; }
    void reset() { cur_ = data_; }
    void clear() { bzero(data_, KFIXEDBUFFERSIZE); }

  private:
    char *data_;
    char *cur_ = nullptr;
};

#endif