#include <bits/stdc++.h>
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
        lock_guard<mutex> lock(mtx);

        if(philosopher%2 == 0){
            fork[(philosopher + 1)%5]->acquire();
            fork[(philosopher)%5]->acquire();
            pickLeftFork();
            pickRightFork();
        }else{
            fork[(philosopher)%5]->acquire();
            fork[(philosopher + 1)%5]->acquire();
            pickRightFork();
            pickLeftFork();
        }

        eat();

        putLeftFork();
        putRightFork();
        fork[(philosopher)%5]->release();
        fork[(philosopher + 1)%5]->release();
    }
};