#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// ------------------ Event Types ------------------
enum class EventType {
    LOCK_REQUEST,
    LOCK_ACQUIRE,
    LOCK_RELEASE,
    READ,
    LOCK_BLOCK,
    WRITE,
    INCREMENT
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
