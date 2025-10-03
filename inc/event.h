#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// ------------------ Event Types ------------------
enum class EventType {
    // Locking events
    LOCK_REQUEST,   // thread requested a lock
    LOCK_ACQUIRE,   // thread acquired lock
    LOCK_RELEASE,   // thread released lock
    LOCK_BLOCK,     // thread was blocked waiting for a lock

    // Memory access events
    READ,           // read a value
    WRITE,          // write a value
    INCREMENT,      // atomic-like increment
    DECREMENT,      // decrement
    DEREF,          // dereferencing a pointer
    FREE,           // freeing memory
    ALLOC,          // allocation (malloc/new)

    // Thread lifecycle events
    THREAD_START,   // thread started
    THREAD_END,     // thread ended
    YIELD,          // thread voluntarily yields
    SLEEP,          // thread sleeps (usleep, nanosleep etc.)

    // Synchronization events
    WAIT,           // wait on condition_variable
    NOTIFY_ONE,     // notify_one called
    NOTIFY_ALL,     // notify_all called
    BARRIER_WAIT,   // barrier synchronization

    // Errors / abnormal
    NULL_ACCESS,    // tried to access null
    USE_AFTER_FREE, // accessed memory after free
    OUT_OF_BOUNDS,  // array/vector out-of-bounds
    DEADLOCK,       // potential deadlock detected
    RACE_COND       // race condition detected
};


// ------------------ Event Struct ------------------
struct Event {
    uint64_t seq;          // sequence number
    uint32_t thread_id;    // small integer ID for the thread
    EventType type;        // operation type
    uint64_t object_id;    // mutex ID or memory address
};

// ------------------ Log Parser ------------------
inline std::vector<Event> parse_log(const std::string &filename) {
    std::vector<Event> events;
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Event ev;
        char T, M;
        int type_num;

        // Example line: "90 T1 3 M1"
        iss >> ev.seq >> T >> ev.thread_id >> type_num >> M >> ev.object_id;

        if (iss) {
            ev.type = static_cast<EventType>(type_num);
            events.push_back(ev);
        }
    }

    // Sort by sequence to ensure deterministic replay
    std::sort(events.begin(), events.end(),
              [](const Event &a, const Event &b) { return a.seq < b.seq; });

    return events;
}
