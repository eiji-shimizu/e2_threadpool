#ifndef Eihire2_Inner_ThreadPool_Win32_INCLUDED
#define Eihire2_Inner_ThreadPool_Win32_INCLUDED

#include <windows.h>

namespace Eihire2::Inner {

    class WorkThreadImpl {
    public:
        WorkThreadImpl(PTP_WORK work);
        ~WorkThreadImpl();

        void submit();

    private:
        PTP_WORK work_;
    };

    class ThreadPoolImpl {
    public:
        ThreadPoolImpl();
        ~ThreadPoolImpl();
        // コピー禁止
        ThreadPoolImpl(const ThreadPoolImpl &) = delete;
        ThreadPoolImpl &operator=(const ThreadPoolImpl &) = delete;
        // ムーブ禁止
        ThreadPoolImpl(ThreadPoolImpl &&) = delete;
        ThreadPoolImpl &operator=(ThreadPoolImpl &&) = delete;

        WorkThreadImpl createThread();

    private:
        PTP_POOL pool_;
        TP_CALLBACK_ENVIRON callBackEnviron_;
        PTP_CLEANUP_GROUP cleanupgroup_;
    };

} // namespace Eihire2::Inner

#endif // Eihire2_Inner_ThreadPool_Win32_INCLUDED
