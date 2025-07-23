#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <vector>

constexpr int N = 5; // Number of philosophers

std::binary_semaphore forks[N] = {
    std::binary_semaphore(1), std::binary_semaphore(1), std::binary_semaphore(1),
    std::binary_semaphore(1), std::binary_semaphore(1)
};

void philosopher(int id) {
    int left = id;
    int right = (id + 1) % N;

    // To avoid deadlock, pick lower-numbered fork first
    int first = std::min(left, right);
    int second = std::max(left, right);

    while (true) {
        std::cout << "Philosopher " << id << " is thinking...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200 + rand() % 300));

        // Pick up forks
        forks[first].acquire();
        forks[second].acquire();

        std::cout << ">>> Philosopher " << id << " is eating...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200 + rand() % 300));

        // Put down forks
        forks[second].release();
        forks[first].release();
    }
}

int main() {
    std::vector<std::thread> philosophers;
    for (int i = 0; i < N; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& t : philosophers)
        t.join();

    return 0;
}
