// crash_test.cpp
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> sharedVec;  // not thread-safe

void worker(int id) {
    for (int i = 0; i < 1000000; i++) {
        sharedVec.push_back(id); // crash risk when reallocating
    }
}

int main() {
    std::vector<std::thread> threads;

    // Multiple threads push into vector without locks
    for (int i = 0; i < 4; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << "Final size = " << sharedVec.size() << std::endl;
    return 0;
}
