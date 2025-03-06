#ifndef LATCH_H
#define LATCH_H
#include "copyable.h"
#include <condition_variable>
#include <mutex>

using std::condition_variable;
using std::mutex;
using std::unique_lock;

class Latch: public noncopyable_and_nonmoveable
{
  public:
    Latch(int count) : count_(count) {}
    void wait()
    {
        std::unique_lock<mutex> lck;
        cv_.wait(lck, [this] { return count_ <= 0; });
    }
    void signal()
    {
        std::unique_lock<mutex> lck;
        if (--count_ <= 0)
            cv_.notify_all();
    }

  private:
    int count_;
    condition_variable cv_;
    mutex mut_;
};
#endif