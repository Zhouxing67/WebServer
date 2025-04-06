#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <array>
#include <charconv>
#include <common.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "FixedBuffer.h"

using std::array;
using std::string;
using std::string_view;
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
    self &operator<<(char *str);

  private:
    Buffer buffer_;
};

template <class T>
inline LogStream &LogStream::operator<<(T val)
{
    // 确保类型可接受
    static_assert(std::is_arithmetic<T>::value || std::is_same<T, string>::value || std::is_same<T, string_view>::value,
                  "Error Type");

    // 数值类型的处理
    if constexpr (std::is_arithmetic<T>::value) {
        array<char, kMaxNumericSize> val_buf{0};
        auto [ptr, ec] = to_chars(val_buf.data(), val_buf.data() + val_buf.size(), val);
        if (ec == std::errc())
            buffer_.append(val_buf.data(), ptr - val_buf.data());
        else
            throw std::runtime_error("Logstream::operator<< : to_chars failed");

    // 如果是字符串类型，直接追加
    } else if constexpr (std::is_same<T, string>::value)
        buffer_.append(val);
    else if constexpr (std::is_same<T, string_view>::value)
        buffer_.append(val.data(), val.size());

    return *this;
}

inline LogStream &LogStream::operator<<(const char *str) { return operator<<(const_cast<char *>(str)); }
inline LogStream &LogStream::operator<<(char *str)
{
    if (str)
        buffer_.append(str, strlen(str));
    else
        buffer_.append("(null)", 6);
    return *this;
}

#endif