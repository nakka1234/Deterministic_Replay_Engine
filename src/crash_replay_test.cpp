#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <cstdlib>
#include "crash_log.h" 
#include "event.h"  // contains Event, EventType, and parse_log()

std::atomic<size_t> global_seq{0};
std::mutex mtx;
std::condition_variable cv;

// shared data
int *ptr = nullptr;
std::atomic<bool> freed{false};
std::vector<Event> events;

void worker(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until it's this thread's turn to replay
        cv.wait(lock, [id]() {
            return global_seq >= events.size() || events[global_seq].thread_id == id;
        });

        if (global_seq >= events.size()) break;

        const Event &ev = events[global_seq];

        // -------- Execute event --------
        switch (ev.type) {
            case EventType::DEREF:
                if (ptr == nullptr || freed.load()) {
                    std::cout << "[T" << id << "] Deref after free! Crash likely!\n";
                } else {
                    int val = *ptr;
                    std::cout << "[T" << id << "] DEREF value=" << val << "\n";
                }
                break;

            case EventType::DECREMENT:
                if (ptr == nullptr || freed.load()) {
                    std::cout << "[T" << id << "] Decrement after free! 💥\n";
                } else {
                    (*ptr)--;
                    std::cout << "[T" << id << "] DECREMENT -> " << *ptr << "\n";
                }
                break;

            case EventType::FREE:
                if (!freed.exchange(true)) {
                    std::cout << "[T" << id << "] FREE memory!\n";
                    free(ptr);
                    ptr = nullptr;
                } else {
                    std::cout << "[T" << id << "] Double free attempt!\n";
                }
                break;

            default:
                std::cout << "[T" << id << "] Unknown event " << static_cast<int>(ev.type) << "\n";
                break;
        }

        // Move to next event
        global_seq++;
        cv.notify_all();
    }
}

int main() {
    // Parse log file (already sorted by seq)
    events = parse_log("log_crash.txt");
    std::cout << "Loaded " << events.size() << " events.\n";

    // Initialize memory
    ptr = (int*)malloc(sizeof(int));
    *ptr = 50;
    freed.store(false);

    // Spawn threads for unique IDs
    std::vector<int> thread_ids;
    for (auto &e : events) {
        if (std::find(thread_ids.begin(), thread_ids.end(), e.thread_id) == thread_ids.end())
            thread_ids.push_back(e.thread_id);
    }

    std::vector<std::thread> threads;
    for (int tid : thread_ids)
        threads.emplace_back(worker, tid);

    for (auto &t : threads)
        t.join();

    std::cout << "Replay finished.\n";
    return 0;
}
