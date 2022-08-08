#ifndef Eihire2_ThreadPool_INCLUDED
#define Eihire2_ThreadPool_INCLUDED

#include "inner/ThreadPool_Win32.h"

namespace Eihire2 {

    class ThreadPool;

    class Work {
    public:
        Work(Inner::WorkImpl workThreadImpl);
        ~Work();

        void start();

    private:
        Inner::WorkImpl workImpl_;
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

        template <typename F, typename... ArgTypes>
        Work addWork(F &&f, ArgTypes &&...args)
        {
            return Work{threadPoolImpl_.createWorkThreadPool(std::forward<F>(f), std::forward<ArgTypes>(args)...)};
        }

    private:
        Inner::ThreadPoolImpl threadPoolImpl_;
    };

} // namespace Eihire2

#endif // Eihire2_ThreadPool_INCLUDED
