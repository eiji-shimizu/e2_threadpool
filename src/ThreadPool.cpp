#include "ThreadPool.h"

namespace Eihire2 {

    /**
     *  WorkThread
     */
    WorkThread::WorkThread(Inner::WorkThreadImpl workThreadImpl)
        : workThreadImpl_{workThreadImpl}
    {
        // noop
    }

    WorkThread::~WorkThread() = default;

    void WorkThread::start()
    {
        workThreadImpl_.submit();
    }

    /**
     *  ThreadPool
     */
    ThreadPool::ThreadPool() = default;
    ThreadPool::~ThreadPool() = default;

    WorkThread ThreadPool::addWorkThread()
    {
        return WorkThread{threadPoolImpl_.createThread()};
    }

} // namespace Eihire2
