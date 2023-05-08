#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// shared variable
int shared_v = 0;

// function to increment shared_v without critical segment
void incrementWithoutCS(int numIncrements) {
    for(int i = 0; i < numIncrements; i++) {
        shared_v++;
    }
}

// function to increment shared_v with critical segment
void incrementWithCS(int numIncrements, mutex* mtx) {
    for(int i = 0; i < numIncrements; i++) {
        mtx->lock();
        shared_v++;
        mtx->unlock();
    }
}

int main() {
    const int numIncrements = 1000000; // 10^6

    // increment without critical segment
    shared_v = 0;
    auto start = chrono::steady_clock::now();
    thread t1(incrementWithoutCS, numIncrements);
    thread t2(incrementWithoutCS, numIncrements);
    t1.join();
    t2.join();
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "Without critical segment: " << shared_v << endl;
    cout << "Time taken: " << chrono::duration<double, milli>(diff).count() << " ms" << endl;

    // increment with critical segment
    shared_v = 0;
    mutex mtx;
    start = chrono::steady_clock::now();
    thread t3(incrementWithCS, numIncrements, &mtx);
    thread t4(incrementWithCS, numIncrements, &mtx);
    t3.join();
    t4.join();
    end = chrono::steady_clock::now();
    diff = end - start;
    cout << "With critical segment: " << shared_v << endl;
    cout << "Time taken: " << chrono::duration<double, milli>(diff).count() << " ms" << endl;
    return 0;
}
