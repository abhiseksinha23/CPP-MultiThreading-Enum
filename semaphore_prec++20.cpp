#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

class Semaphore {
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    Semaphore(int count = 0) : count(count) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return count > 0; });
        --count;
    }

    void signal() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }
};
