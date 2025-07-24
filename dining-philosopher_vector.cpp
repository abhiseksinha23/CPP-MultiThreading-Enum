#include<bits/stdcc+.h>
using namespace std;

class DiningPhilosophers {
    mutex mtx;
    // binary_semaphore fork[5] = {
    //     binary_semaphore(1),
    //     binary_semaphore(1),
    //     binary_semaphore(1),
    //     binary_semaphore(1),
    //     binary_semaphore(1)
    // };
    vector<unique_ptr<binary_semaphore>>fork;
public:
    DiningPhilosophers() {
        for (int i = 0; i < 5; ++i) {
            fork.emplace_back(make_unique<binary_semaphore>(1));
        }
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        {
            lock_guard<mutex> lock(mtx);
            fork[(philosopher + 1)%5]->acquire();
            fork[philosopher]->acquire();
        }
        
        pickLeftFork();
        pickRightFork();
        eat();

        putLeftFork();
        fork[(philosopher + 1)%5]->release();
        putRightFork();
        fork[philosopher]->release();
    }
};