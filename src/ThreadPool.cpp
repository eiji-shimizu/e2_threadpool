#include "ThreadPool.h"

namespace Eihire2 {

    /**
     *  Work
     */
    Work::Work(Inner::WorkImpl workImpl)
        : workImpl_{workImpl}
    {
        // noop
    }

    Work::~Work() = default;

    void Work::start()
    {
        workImpl_.submit();
    }

    /**
     *  ThreadPool
     */
    ThreadPool::ThreadPool() = default;
    ThreadPool::~ThreadPool() = default;

    Work ThreadPool::addWork()
    {
        return Work{threadPoolImpl_.createWorkThreadPool()};
    }

} // namespace Eihire2
