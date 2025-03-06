#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <array>
#include <charconv>
#include <common.h>
#include <cstring>
#include <stdexcept>

#include "FixedBuffer.h"

using std::array;
using std::string;
using std::to_chars;

static const int kMaxNumericSize = 48;

class LogStream
{
  public:
    using self = LogStream;
    using Buffer = FixedBuffer;

    DISALLOW_COPY_AND_MOV(LogStream)
    LogStream() = default;
    ~LogStream() = default;

    void append(const char *data, int len) { buffer_.append(data, len); }
    const Buffer &buffer() const { return buffer_; }
    void resetBuffer()
    {
        buffer_.clear();
        buffer_.reset();
    }

    template <class NumericalType>
    self &operator<<(NumericalType val);
    
    self &operator<<(const char *str);
    self &operator<<(bool bool_);

  private:
    Buffer buffer_;
};

template <class NumericalType>
inline LogStream &LogStream::operator<<(NumericalType val)
{
    // 确保类型是算术类型
    static_assert(std::is_arithmetic<NumericalType>::value || std::is_same<NumericalType, std::string>::value,
                  "只允许数值类型、字符类型、bool类型、字符串类型");

    // 数值类型的处理
    if constexpr (std::is_arithmetic<NumericalType>::value) {
        array<char, kMaxNumericSize> val_buf{0};
        auto [ptr, ec] = to_chars(val_buf.data(), val_buf.data() + val_buf.size(), val);
        if (ec == std::errc()) {
            buffer_.append(val_buf.data(), ptr - val_buf.data());
        } else {
            throw std::runtime_error("Logstream::operator<< : to_chars failed");
        }
    } else if constexpr (std::is_same<NumericalType, string>::value) {
        // 如果是字符串类型，直接追加
        buffer_.append(val);
    }

    return *this;
}

inline LogStream &LogStream::operator<<(const char *str)
{
    if (str)
        buffer_.append(str, strlen(str));
    else
        buffer_.append("(null)", 6);
    return *this;
}

inline LogStream &LogStream::operator<<(bool bool_)
{
    if (bool_)
        return *this << ("true");
    return *this << ("false");
}

#endif