#ifndef LOG_STREAM_H
#define LOG_STREM_H

#include <array>
#include <charconv>
#include <common.h>
#include <stdexcept>
#include <string.h>
#include <string>

using std::array;
using std::string;
using std::string_view;
using std::to_chars;

static const int KFIXEDBUFFERSIZE = 4096;
static const int kMaxNumericSize = 48;

class FixedBuffer {
public:
  FixedBuffer() = default;
  ~FixedBuffer() = default;

  void append(const char *buf, int len);
  void append(string_view buf);
  void append(char c);

  const char *data() const { return data_; }
  int size() const { return cur_ - data_; }

  char *current() { return cur_; }
  const char *end() const { return data_ + KFIXEDBUFFERSIZE; }
  int avail() const { return end() - cur_; }
  void bias(int len) { cur_ += len; }

  void reset() { cur_ = data_; }
  void clear() { bzero(data_, KFIXEDBUFFERSIZE); }

private:
  char data_[KFIXEDBUFFERSIZE] = {0};
  char *cur_ = data_;
};

class LogStream {
public:
  typedef LogStream self;
  typedef FixedBuffer Buffer;

  DISALLOW_COPY_AND_MOV(LogStream);
  LogStream() = default;
  ~LogStream() = default;

  void append(const char *data, int len) { buffer_.append(data, len); }
  const Buffer &buffer() const { return buffer_; }
  void resetBuffer() {
    buffer_.clear();
    buffer_.reset();
  }

  template <class NumericalType> self &operator<<(NumericalType val);

private:
  Buffer buffer_;
};
#endif

template <class NumericalType>
inline LogStream &LogStream::operator<<(NumericalType val) {
  static_assert(std::is_arithmetic<NumericalType>::value,
                "只允许数值类型,字符类型,bool类型,字符串;");
  array<char, kMaxNumericSize> val_buf;
  auto [ptr, ec] =
      to_chars(val_buf.data(), val_buf.data() + val_buf.size(), val);
  if (ec == std::errc())
    buffer_.append(val_buf.data(), ptr - val_buf.data());
  else
    throw std::runtime_error("Logstream::operator<< : to_chars failed");

  return *this;
}

template <> inline LogStream &LogStream::operator<< <bool>(bool bool_) {
  string_view true_ = "true", false_ = "false_";
  if (bool_)
    buffer_.append(true_);
  else
    buffer_.append(false_);
  return *this;
}

template <> inline LogStream &LogStream::operator<< <string>(string str) {
  buffer_.append(string_view(str));
  return *this;
}
