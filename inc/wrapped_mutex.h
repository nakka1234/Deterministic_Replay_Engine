#include <mutex>
#include "logger.h"



class InstrumentedMutex {
    std::mutex m;
    Logger *logger;
    int threadId;
public:
    InstrumentedMutex(Logger* l, int id) : logger(l), threadId(id) {
        logger->log(threadId, EventType::LOCK_REQUEST, 0);
        m.lock();
        logger->log(threadId, EventType::LOCK_ACQUIRE, 0);
    }

    ~InstrumentedMutex() {
        m.unlock();
        logger->log(threadId, EventType::LOCK_RELEASE, 0);
    }
};

