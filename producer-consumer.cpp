#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>

constexpr int BUFFER_SIZE = 5;

std::queue<int> buffer;
std::mutex mtx;

std::counting_semaphore<BUFFER_SIZE> empty_slots(BUFFER_SIZE); // Initially all empty
std::counting_semaphore<BUFFER_SIZE> full_slots(0);            // Initially none full

void producer(int id) {
    int item = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 300)); // simulate work
        item++;

        empty_slots.acquire(); // Wait if buffer is full

        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(item);
            std::cout << "[Producer " << id << "] produced: " << item << " | Buffer size: " << buffer.size() << "\n";
        }

        full_slots.release(); // Notify a consumer
    }
}

void consumer(int id) {
    while (true) {
        full_slots.acquire(); // Wait if buffer is empty

        int item;
        {
            std::lock_guard<std::mutex> lock(mtx);
            item = buffer.front();
            buffer.pop();
            std::cout << "    [Consumer " << id << "] consumed: " << item << " | Buffer size: " << buffer.size() << "\n";
        }

        empty_slots.release(); // Notify a producer

        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 300)); // simulate processing
    }
}

int main() {
    std::thread producers[] = {
        std::thread(producer, 1),
        std::thread(producer, 2)
    };

    std::thread consumers[] = {
        std::thread(consumer, 1),
        std::thread(consumer, 2)
    };

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    return 0;
}
