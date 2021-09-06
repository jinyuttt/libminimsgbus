#pragma once
#include <mutex>
#include <condition_variable>
using namespace std;
class bussemaphore
{
public:
    bussemaphore(int value = 1) : count{ value }, wakeups{ 0 }  { }

    void wait() {
        std::unique_lock <std::mutex > lock{ mutex };
        if (--count < 0) {  // count is not enough ?
            condition.wait(lock, [&]() -> bool{ return wakeups > 0; });  // suspend and wait ...
            --wakeups;   // ok, me wakeup !
        }
    }
    void  signal() {
        std::lock_guard <std::mutex > lock{ mutex };
        if (++count <= 0) {  // have some thread suspended ?
            ++wakeups;
            condition.notify_one();  // notify one !
        }
    }
    void resize(int value)
    {
        count = value;
    }
private:
    int count;
    int wakeups;
    std::mutex mutex;
    std::condition_variable condition;
};


