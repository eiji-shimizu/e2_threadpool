#ifndef Eihire2_Inner_ThreadPool_Win32_INCLUDED
#define Eihire2_Inner_ThreadPool_Win32_INCLUDED

#include <windows.h>

#include <functional>

namespace Eihire2::Inner {

    template <typename>
    class WorkCallback;

    template <typename R, typename... ArgTypes>
    class WorkCallback<R(ArgTypes...)> {
    public:
        WorkCallback(std::function<R(ArgTypes...)> &&function)
            : function_{std::forward<std::function<R(ArgTypes...)>>(function)}
        {
            // noop
        }

        ~WorkCallback() = default;

        static VOID CALLBACK wrapper(PTP_CALLBACK_INSTANCE instance,
                                     PVOID parameter,
                                     PTP_WORK work)
        {
            try {
                UNREFERENCED_PARAMETER(instance);
                UNREFERENCED_PARAMETER(parameter);
                UNREFERENCED_PARAMETER(work);

                WorkCallback<R(ArgTypes...)> *wcPtr = static_cast<WorkCallback<R(ArgTypes...)> *>(parameter);
                wcPtr->function_();
            }
            catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
            catch (...) {
                std::cout << "unexpected error." << std::endl;
            }
        }

        // rivate:
        std::function<R(ArgTypes...)> function_;
    };

    class WorkImpl {
    public:
        WorkImpl(PTP_WORK work);
        ~WorkImpl();

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

        WorkImpl createWorkThreadPool();

    private:
        PTP_POOL pool_;
        TP_CALLBACK_ENVIRON callBackEnviron_;
        PTP_CLEANUP_GROUP cleanupgroup_;
    };

} // namespace Eihire2::Inner

#endif // Eihire2_Inner_ThreadPool_Win32_INCLUDED
