#pragma once
#include <cstdint>
#include <string>

enum class EventType {
    LOCK_REQUEST,
    LOCK_ACQUIRE,
    LOCK_RELEASE,
    READ,
    LOCK_BLOCK,
    WRITE,
    INCREMENT
};

struct Event {
    uint64_t seq;          // sequence number
    uint32_t thread_id;    // small integer ID for the thread
    EventType type;
    uint64_t object_id;    // mutex ID
};
