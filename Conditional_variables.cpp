#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::queue<int> buffer;
const unsigned int MAX_SIZE = 5;

std::mutex mtx;
std::condition_variable cv;

void producer() {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [](){ return buffer.size() < MAX_SIZE; });

        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;

        cv.notify_all();  // notify consumer
    }
}

void consumer() {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [](){ return !buffer.empty(); });

        int val = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << val << std::endl;

        cv.notify_all();  // notify producer
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
    return 0;
}
