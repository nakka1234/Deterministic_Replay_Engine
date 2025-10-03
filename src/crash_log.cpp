#include "crash_log.h"

CrashLogger::CrashLogger(const std::string &filename) {
    logfile.open(filename, std::ios::out | std::ios::trunc); 
}

CrashLogger::~CrashLogger() {
    logfile.flush();
    logfile.close();
}

void CrashLogger::log(int thread_id, EventType type, long value) {
    std::lock_guard<std::mutex> lock(file_mtx);

    std::ostringstream oss;
    oss << "T" << thread_id
        << " EVENT=" << static_cast<int>(type)
        << " VALUE=" << value
        << "\n";

    logfile << oss.str();
    logfile.flush();   // ✅ flush every time — safer for crashes
}
