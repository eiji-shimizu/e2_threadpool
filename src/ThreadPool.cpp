#include "ThreadPool.h"

namespace Eihire2 {

    WorkThread::WorkThread(Inner::WorkThreadImpl workThreadImpl)
        : workThreadImpl_{workThreadImpl}
    {
        // noop
    }

    WorkThread::~WorkThread()
    {
        // noop
    }

    void WorkThread::start()
    {
        workThreadImpl_.submit();
    }

    ThreadPool::ThreadPool()
    {
        // noop
    }

    ThreadPool::~ThreadPool()
    {
        // noop
    }

    WorkThread ThreadPool::addWorkThread()
    {
        return WorkThread{threadPoolImpl_.createThread()};
    }

} // namespace Eihire2
