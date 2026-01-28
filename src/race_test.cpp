// race_test.cpp
#include <iostream>
#include <thread>
#include <vector>
#include "logger.h"

int counter = 0; // shared but not atomic

Logger logger_1("log_race.txt");

// Worker function
void worker(int id) {
    for (int i = 0; i < 100; i++) {   // small loop so log is readable
        int temp = counter;                 // load
        logger_1.log(id, (EventType )3, temp);

        temp = temp + 1;                    // increment
        logger_1.log(id, (EventType )6, temp);

        counter = temp;                     // store
        logger_1.log(id, (EventType )5, counter);
    }
}

int main() {
    std::vector<std::thread> threads;

    // Two threads → race
    for (int i = 0; i < 2; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << "Expected = 2000, Actual = " << counter << std::endl;
    return 0;
}
