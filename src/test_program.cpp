#include <iostream>
#include <thread>
#include <vector>

#include "logger.h"
#include "event.h"

// Shared counter (not atomic → race condition will appear)
int counter = 0;

Logger logger("log.txt");

// Worker function
void worker(int id) {
    for (int i = 0; i < 100; i++) {   // small loop so log is readable
        int temp = counter;                 // load
        logger.log(id, (EventType )0, temp);

        temp = temp + 1;                    // increment
        logger.log(id, (EventType )1, temp);

        counter = temp;                     // store
        logger.log(id, (EventType )2, counter);
    }
}

int main() {
    

    std::vector<std::thread> threads;
    

    // Start 2 threads (easier to see interleaving in logs)
    for (int i = 0; i < 2; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << "Final counter = " << counter << std::endl;

    
    return 0;
}
