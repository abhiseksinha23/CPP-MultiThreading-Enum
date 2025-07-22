#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <vector>
#include <chrono>
#include <string>
using namespace std;

class Washroom {
private:
    std::counting_semaphore<3> capacity_semaphore{3};  // Max 3 people
    std::mutex mtx;
    std::condition_variable cv;

    int inside_count = 0;
    std::string current_party = "";

public:
    void enter(const std::string& party, int id) {
        // Wait until either washroom is empty or same party is inside
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() {
            return current_party == "" || current_party == party;
        });

        if (inside_count == 0)
            current_party = party;

        ++inside_count;
        lock.unlock();

        capacity_semaphore.acquire(); // Semaphore gate
        std::cout << "Person " << id << " from " << party << " entered. Inside count = " << inside_count << "\n";
    }

    void leave(const std::string& party, int id) {
        capacity_semaphore.release(); // Leaving slot

        std::unique_lock<std::mutex> lock(mtx);
        --inside_count;
        std::cout << "Person " << id << " from " << party << " left. Remaining = " << inside_count << "\n";

        if (inside_count == 0) {
            current_party = "";
            cv.notify_all(); // Allow other party to enter
        }
    }
};

void person(Washroom& washroom, const std::string& party, int id) {
    washroom.enter(party, id);
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 300));
    washroom.leave(party, id);
}

int main() {
    Washroom washroom;
    std::vector<std::thread> threads;

    for (int i = 0; i < 12; ++i) {
        std::string party = (i % 2 == 0) ? "PartyA" : "PartyB";
        threads.emplace_back(person, std::ref(washroom), party, i);
    }

    for (auto& t : threads)
        t.join();

    return 0;
}
