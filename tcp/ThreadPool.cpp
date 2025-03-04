#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int concurrency)
{
    for (unsigned int i = 0; i < concurrency; i++) {
        threads_.emplace_back(thread([this] {
            while (true) {
                function<void()> task;
                {
                    unique_lock<mutex> ulock(mut_);
                    //等待stop_ 或者 ！tasks_.empty()
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


