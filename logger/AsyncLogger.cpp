#include "AsyncLogger.h"
#include "FixedBuffer.h"
#include "LogFile.h"
#include <memory>
#include <mutex>

using std::lock_guard;
using std::make_unique;
using std::unique_lock;

void AsyncLogger::output(const char *message, size_t len)
{
    lock_guard<mutex> lock(mut_);

    if (!currentBuf_) {
        currentBuf_ = get_buffer();
    }

    if (currentBuf_->avail() < len) {
        buffersToWrite_.push_back(std::move(currentBuf_));
        cv_.notify_one();
        currentBuf_ = get_buffer();
    }
    currentBuf_->append(message, len);
}

void AsyncLogger::start()
{
    if (!running_) {
        running_ = true;
        workerThread_ = thread(&AsyncLogger::worker_thread_func, this);
    }
}

void AsyncLogger::stop()
{
    if (running_) {
        running_ = false;
        cv_.notify_all();

        if (workerThread_.joinable())
            workerThread_.join();

        // 刷新剩余日志
        lock_guard<mutex> lock(mut_);
        if (currentBuf_ && !currentBuf_->empty())
            buffersToWrite_.push_back(std::move(currentBuf_));
        write_buffers_to_file(buffersToWrite_);
    }
}

AsyncLogger::BufferPtr AsyncLogger::get_buffer()
{
    if (!freeBufs_.empty()) {
        BufferPtr buf = std::move(freeBufs_.back());
        freeBufs_.pop_back();
        buf->bzero();
        return buf;
    }

    // 创建预分配空间的缓冲区
    auto buf = make_unique<Buffer>(KFIXED_LARGER_BUFFER_SIZE);
    return buf;
}

void AsyncLogger::worker_thread_func()
{
    fp_ = make_unique<LogFile>();
    while (running_ || !buffersToWrite_.empty())
    {
        vector<BufferPtr> readyBuffers;

        {
            unique_lock<mutex> lock(mut_);
            cv_.wait(lock, [this] { return !buffersToWrite_.empty() || !running_; });

            readyBuffers.swap(buffersToWrite_);
        }

        if (!readyBuffers.empty()) {
            write_buffers_to_file(readyBuffers);

            // 回收缓冲区
            lock_guard<mutex> lock(mut_);
            for (auto &buf : readyBuffers)
                freeBufs_.push_back(std::move(buf));
        }
    }
}

void AsyncLogger::write_buffers_to_file(const vector<BufferPtr> &buffers)
{
    for (const auto &buf : buffers) {
        if (!buf->empty()) {
            fp_->Write(buf->data(), buf->size());
            if (fp_->writtenbytes() >= LogFile::LOG_FILE_BASE_SIZE)
                fp_.reset(new LogFile());
        }
    }
}