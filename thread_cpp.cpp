#include <iostream> // for input/output operations
#include <thread> // for including thread library
#include <chrono> // for time-related functions
using namespace std;


void taskA(int count){
    for(int i = 0; i < count; i++){
        cout << "Task A: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100)); // simulate work
    }
}

void taskB(){
    for(int i = 0; i < 100; i++){
        cout << "Task B: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100)); // simulate work
    }
}

int main(){
    int count = 50; // Number of iterations for task A

    // Create threads
    thread t1(taskA, count); // Pass count to taskA
    thread t2(taskB); // No parameters for taskB

    // Wait for threads to finish
    t1.join();
    t2.join(); // Ensure both threads complete before exiting main

    cout << "All tasks completed!" << endl;

    return 0;
}