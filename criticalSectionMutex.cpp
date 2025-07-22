#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

void printNumbers(int id) {
    for (int i = 0; i < 5; ++i) {
        mtx.lock();  // Lock the mutex
        cout << "Thread " << id << " -> " << i << endl;
        mtx.unlock();  // Unlock the mutex
    }
}

int main() {
    thread t1(printNumbers, 1);
    thread t2(printNumbers, 2);

    t1.join();
    t2.join();

    return 0;
}
