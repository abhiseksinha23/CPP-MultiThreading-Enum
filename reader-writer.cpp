#include <iostream>
#include <thread>
#include <semaphore>
#include <mutex>
#include <chrono>

std::mutex mtx;                    // to protect read_count
int read_count = 0;                // number of active readers

std::binary_semaphore rw_mutex(1); // ensures mutual exclusion for writers/readers
std::binary_semaphore mutex(1);    // controls access to read_count

void reader(int id) {
    while (true) {
        // Entry Section
        mutex.acquire();
        read_count++;
        if (read_count == 1) {
            rw_mutex.acquire(); // First reader blocks writers
        }
        mutex.release();

        // Critical Section
        std::cout << "Reader " << id << " is reading.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 300));

        // Exit Section
        mutex.acquire();
        read_count--;
        if (read_count == 0) {
            rw_mutex.release(); // Last reader releases writer lock
        }
        mutex.release();

        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // simulate other work
    }
}

void writer(int id) {
    while (true) {
        // Entry Section
        rw_mutex.acquire(); // Ensure exclusive access

        // Critical Section
        std::cout << ">>> Writer " << id << " is writing...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 400));

        // Exit Section
        rw_mutex.release();

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate other work
    }
}

int main() {
    std::thread readers[] = {
        std::thread(reader, 1),
        std::thread(reader, 2),
        std::thread(reader, 3)
    };

    std::thread writers[] = {
        std::thread(writer, 1),
        std::thread(writer, 2)
    };

    for (auto& t : readers) t.join();
    for (auto& t : writers) t.join();

    return 0;
}
