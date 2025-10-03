#include <iostream>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <unistd.h> // sleep
#include "crash_log.h"

int *ptr = nullptr;
std::atomic<bool> freed{false};  // track if already freed
CrashLogger crashLogger("log_crash.txt");

void worker(int id) {
    while (true) {
        if (freed.load()) {
            // someone already freed the memory
            std::cout << "[Thread " << id << "] ptr already freed!\n";
            break;
        }

        if (ptr) {
            int val = *ptr;
            crashLogger.log(id, EventType::DEREF, val);

            if (val > 0) {
                (*ptr)--; // decrement
                crashLogger.log(id, EventType::DECREMENT, *ptr);

                std::cout << "[Thread " << id << "] Decremented -> " << *ptr << "\n";

                if (*ptr == 0) {
                    std::cout << "[Thread " << id << "] Value hit 0, freeing memory!\n";
                    free(ptr);   // 💥 unsafe on purpose for crash simulation
                    ptr = nullptr;
                    freed.store(true);
                    crashLogger.log(id, EventType::FREE, 0);
                    break;
                }
            } else {
                std::cout << "[Thread " << id << "] Value already non-positive\n";
               
                break;
            }
        } else {
            std::cout << "[Thread " << id << "] Null pointer access attempt!\n";
           
            break;
        }
    }
}

int main() {
    srand(time(nullptr));
    ptr = (int*)malloc(sizeof(int));
    *ptr = 50;   // start value

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    std::thread t3(worker, 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Finished.\n";
    return 0;
}
