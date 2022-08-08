#ifndef Eihire2_ThreadPool_INCLUDED
#define Eihire2_ThreadPool_INCLUDED

#include "inner/ThreadPool_Win32.h"

namespace Eihire2 {

    class ThreadPool;

    class WorkThread {
    public:
        WorkThread(Inner::WorkThreadImpl workThreadImpl);
        ~WorkThread();

        void start();

    private:
        Inner::WorkThreadImpl workThreadImpl_;
    };

    class ThreadPool {
    public:
        ThreadPool();
        ~ThreadPool();
        // コピー禁止
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;
        // ムーブ禁止
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;

        WorkThread addWorkThread();

    private:
        Inner::ThreadPoolImpl threadPoolImpl_;
    };

} // namespace Eihire2

#endif // Eihire2_ThreadPool_INCLUDED
