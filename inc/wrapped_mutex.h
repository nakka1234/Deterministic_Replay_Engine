#include <mutex>
#include "logger.h"

class InstrumentedMutex {
    std::mutex m;
    Logger *logger;
    uint64_t id; // unique mutex identifier
public:
    InstrumentedMutex(Logger* l, uint64_t mid)
        : logger(l), id(mid) {}

    void lock(int threadId) {
        logger->log(threadId, EventType::LOCK_REQUEST, id);

        if (!m.try_lock()) {
            logger->log(threadId, EventType::LOCK_BLOCK, id);
            m.lock();
        }

        logger->log(threadId, EventType::LOCK_ACQUIRE, id);
    }

    void unlock(int threadId) {
        m.unlock();
        logger->log(threadId, EventType::LOCK_RELEASE, id);
    }
};

// RAII guard
class InstrumentedLockGuard {
    InstrumentedMutex &m;
    int tid;
public:
    InstrumentedLockGuard(InstrumentedMutex &mutex, int threadId)
        : m(mutex), tid(threadId) {
        m.lock(tid);
    }

    ~InstrumentedLockGuard() {
        m.unlock(tid);
    }
};
