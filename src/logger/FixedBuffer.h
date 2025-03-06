#ifndef FIXED_BUFFER_H
#define FIXED_BUFFER_H
#include <cstdio>
#include <string>
#include <strings.h>
using std::string;
using std::string_view;

static const int KFIXED_BUFFER_SIZE = 4096;
static const int KFIXED_LARGER_BUFFER_SIZE = 8192;

class FixedBuffer
{
  public:
    FixedBuffer(int capacity = KFIXED_BUFFER_SIZE) : capacity_(capacity), data_(new char[capacity_]), cur_(data_){};
    ~FixedBuffer()
    {
        delete[] data_;
        data_ = cur_ = nullptr;
    };
    FixedBuffer(const FixedBuffer&other);
    FixedBuffer &operator=(const FixedBuffer &other);

    FixedBuffer(FixedBuffer &&other) noexcept;
    FixedBuffer& operator=(FixedBuffer &&other) noexcept;

    void append(const char *buf, int len);
    void append(const string &buf);
    void append(char c);

    const char *data() const { return data_; }
    int size() const { return static_cast<int>(cur_ - data_); }
    bool empty() const {return !(size() > 0);}

    int avail() const { return data_ + capacity_ - cur_; }
    void reset() { cur_ = data_; }
    void clear() { ::bzero(data_, capacity_); }

    void bzero()
    {
        reset();
        clear();
    }

  private:
    const int capacity_;
    char *data_;
    char *cur_ = nullptr;

    void move_help(FixedBuffer &other)
    {
        cur_ = other.cur_;
        data_ = other.data_;
        other.cur_ = other.data_ = nullptr;
    }

};

#endif