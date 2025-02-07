#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <future>
#include <memory>


using std::thread;
using std::lock_guard;
using std::unique_lock;
using std::mutex;
using std::condition_variable;
using std::queue;
using std::vector;
using std::function;
using std::future;
using std::packaged_task;
using std::invoke_result;
using std::invoke_result_t;
using std::forward;
using std::make_shared;

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
    
    template<class F, class... Args>
    auto en_que(F &&f, Args&&... args) -> future<invoke_result_t<F, Args...>>;

};

template<class F, class... Args>
auto ThreadPool::en_que(F &&f, Args&&... args)->future<invoke_result_t<F, Args...>>
{
    using result_type = invoke_result_t<F, Args...>;
    auto task = make_shared< packaged_task<result_type(void)> >(  //使用智能指针
        std::bind(forward<F>(f), forward<Args>(args)...)  //完美转发参数
    );
    auto fut = task->get_future();
    {
        lock_guard<mutex> loc(mut_);
        if (stop_)
            throw std::runtime_error("ThreadPool stopped!");

        tasks_.emplace([task] {
            (*task)();
            });
    }
    cv_.notify_one();
    return fut;
}
#endif