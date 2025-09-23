#include "Logger.h"
#include <iostream>

Logger::Logger(const std::string &filename) {
    logfile.open(filename);
}

Logger::~Logger() {
    logfile.close();
}

void Logger::log(uint32_t thread_id, EventType type, uint64_t object_id) {
    uint64_t seq = seq_counter.fetch_add(1);

    std::lock_guard<std::mutex> lock(log_mtx);
    logfile << seq << " T" << thread_id
            << " " << static_cast<int>(type)
            << " M" << object_id << "\n";
}
