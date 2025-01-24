#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int concurrency)
{
    for (int i = 0; i < concurrency; i++) {
        threads_.emplace_back(thread([this] {
            while (true) {
                function<void()> task;
                {
                    unique_lock<mutex> ulock(mut_);
                    cv_.wait(ulock, [this] {return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty())
                        return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
            }));
    }

}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mut_);
        stop_ = true;
    }
    cv_.notify_all();
    for (std::thread &th : threads_) {
        if (th.joinable())
            th.join();
    }
}

void ThreadPool::en_que(function<void()> &&func)
{
    {
        unique_lock ulock(mut_);
        if (stop_)
            throw std::runtime_error("ThreadPool stopped!!!");
        tasks_.push(std::move(func));
    }
    cv_.notify_one();
}