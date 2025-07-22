#include <iostream>
#include <semaphore>
#include <thread>
using namespace std;

std::counting_semaphore<3> sem(3);  // Max 3 threads allowed at once

void access(int id) {
    sem.acquire();  // ↓ wait()
    std::cout << "Thread " << id << " is accessing resource\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << id << " is done\n";
    sem.release();  // ↑ signal()
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 1; i <= 10; ++i)
        //threads.emplace_back(access, i);
        threads.push_back(thread(access, i));

    for (auto& t : threads) t.join();
    return 0;
}
