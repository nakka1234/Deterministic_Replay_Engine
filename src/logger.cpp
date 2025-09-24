#include "Logger.h"
#include <iostream>

thread_local std::vector<std::string> Logger::thread_buffer;

Logger::Logger(const std::string &filename) {
    logfile.open(filename);
}

Logger::~Logger() {
    flush();  // flush this thread’s buffer
    logfile.close();
}

void Logger::log(uint32_t thread_id, EventType type, uint64_t object_id) {
    uint64_t seq = seq_counter.fetch_add(1);

    std::ostringstream oss;
    oss << seq << " T" << thread_id
        << " " << static_cast<int>(type)
        << " M" << object_id << "\n";

    thread_buffer.push_back(oss.str());

    if (thread_buffer.size() >= BUFFER_LIMIT) {
        flush();
    }
}

void Logger::flush() {
    if (thread_buffer.empty()) return;

    std::lock_guard<std::mutex> lock(file_mtx);
    for (const auto &line : thread_buffer) {
        logfile << line;
    }
    logfile.flush();
    thread_buffer.clear();
}
