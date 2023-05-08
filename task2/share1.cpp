#include <mutex>
#include <iostream>
#include <thread>

std::mutex mtx; // create a mutex

void increment_with_critical(int& v) {
    for (int i = 0; i < 1000000; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // lock the mutex
        v++; // increment the shared variable
    }
}

void increment_without_critical(int& v) {
    for (int i = 0; i < 1000000; ++i) {
        v++; // increment the shared variable
    }
}

int main() {
    int v = 0;

    // increment with critical segment
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread t1(increment_with_critical, std::ref(v));
    std::thread t2(increment_with_critical, std::ref(v));
    t1.join();
    t2.join();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "With critical segment: " << v << "\n";
    std::cout << "Time taken: " << duration.count() << " ms\n";

    // increment without critical segment
    v = 0;
    start_time = std::chrono::high_resolution_clock::now();
    std::thread t3(increment_without_critical, std::ref(v));
    std::thread t4(increment_without_critical, std::ref(v));
    t3.join();
    t4.join();
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Without critical segment: " << v << "\n";
    std::cout << "Time taken: " << duration.count() << " ms\n";

    return 0;
}
