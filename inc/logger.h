#pragma once
#include "event.h"
#include <mutex>
#include <fstream>
#include <atomic>

class Logger {
    std::ofstream logfile;
    std::atomic<uint64_t> seq_counter{0};
    std::mutex log_mtx;  // keep it simple for MVP

public:
    Logger(const std::string &filename);
    ~Logger();

    void log(uint32_t thread_id, EventType type, uint64_t object_id);
};
