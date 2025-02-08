#include "../src/ThreadPool.h"
#include <iostream>

using std::cout;
using std::endl;

mutex mut;

//g++ test/threadPool_test.cpp src/ThreadPool.cpp -o threadPool_test
int main()
{
    ThreadPool *pool = new ThreadPool;
    for (int i = 0; i < 16; i++)
    {
        pool->en_que([i] {
            lock_guard lg(mut);
            cout << "print " << i << endl;
            });
    }
}