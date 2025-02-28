#pragma once

#include "FixedBuffer.h"
#include "LogFile.h"
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using std::atomic;
using std::condition_variable;
using std::mutex;
using std::thread;
using std::vector;
using std::unique_ptr;

    class AsyncLogger
{
  public:
    using Buffer = FixedBuffer;
    using BufferPtr = std::unique_ptr<Buffer>;

    AsyncLogger() = default;
    ~AsyncLogger() { stop(); }

    // 禁止拷贝和移动
    AsyncLogger(const AsyncLogger &) = delete;
    AsyncLogger &operator=(const AsyncLogger &) = delete;
    AsyncLogger(AsyncLogger &&) = delete;
    AsyncLogger &operator=(AsyncLogger &&) = delete;

    void output(const char *message, size_t len);
    void start();
    void stop();

  private:
    BufferPtr get_buffer();
    void worker_thread_func();
    void write_buffers_to_file(const std::vector<BufferPtr> &buffers);

    BufferPtr currentBuf_;
    vector<BufferPtr> buffersToWrite_;
    vector<BufferPtr> freeBufs_;

    mutex mut_;
    condition_variable cv_;
    atomic<bool> running_{false};
    thread workerThread_;
    
    unique_ptr<LogFile> fp_;
};
