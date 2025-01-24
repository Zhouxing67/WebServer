#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

using std::thread;
using std::lock_guard;
using std::unique_lock;
using std::mutex;
using std::condition_variable;
using std::queue;
using std::vector;
using std::function;

class ThreadPool {
private:
    vector<thread> threads_;
    queue<function<void()>> tasks_;
    mutex mut_;
    condition_variable cv_;
    bool stop_ = false;
public:
    ThreadPool(unsigned int concurrency = thread::hardware_concurrency());
   
    ~ThreadPool();
    ThreadPool(const ThreadPool &other) = delete;
    const ThreadPool &operator=(const ThreadPool &other) = delete;
    void en_que(function<void()> &&);

};
#endif