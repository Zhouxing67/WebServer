#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <stdexcept>
#include <unistd.h>

static const time_t FlushInterval = 3;

class LogFile
{
  public:
    LogFile();
    ~LogFile();

    constexpr static int LOG_FILE_BASE_SIZE = 1024 * 8;
    
    void Write(const char *data, int len);
    void Flush();
    int64_t writtenbytes() const;

  private:
    FILE *fp_;

    int64_t written_bytes_ = 0;
    time_t lastwrite_ = 0;
    time_t lastflush_ = 0;
};
#endif