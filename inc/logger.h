#pragma once
#include "event.h"
#include <mutex>
#include <fstream>
#include <atomic>
#include <vector>
#include <sstream>
#include <thread>

class Logger {
    std::ofstream logfile;
    std::atomic<uint64_t> seq_counter{0};
    std::mutex file_mtx;  

    // Each thread has its own buffer
    static thread_local std::vector<std::string> thread_buffer;
    static constexpr size_t BUFFER_LIMIT = 100; // flush when 100 entries

public:
    Logger(const std::string &filename);
    ~Logger();

    void log(uint32_t thread_id, EventType type, uint64_t object_id);
    void flush();  // flush one thread’s buffer
};
