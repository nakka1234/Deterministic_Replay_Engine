#ifndef CRASH_LOGGER_H
#define CRASH_LOGGER_H

#include <fstream>
#include <mutex>
#include <string>
#include <sstream>
#include "event.h"
#include <atomic>

class CrashLogger {
public:
    CrashLogger(const std::string &filename);
    ~CrashLogger();
    

    void log(int thread_id, EventType type, long value);

private:
    std::ofstream logfile;
    std::mutex file_mtx;  // protects logfile writes
    std::atomic<uint64_t> seq_counter{0};
};

#endif // CRASH_LOGGER_H
