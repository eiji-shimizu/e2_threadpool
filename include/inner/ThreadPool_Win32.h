#ifndef Eihire2_Inner_ThreadPool_Win32_INCLUDED
#define Eihire2_Inner_ThreadPool_Win32_INCLUDED

#include <windows.h>

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Eihire2::Inner {

    template <typename>
    class WorkCallback;

    template <typename R, typename... ArgTypes>
    class WorkCallback<R(ArgTypes...)> {
    public:
        WorkCallback(std::function<R(ArgTypes...)> &&function, ArgTypes... args)
            : function_{std::forward<std::function<R(ArgTypes...)>>(function)},
              args_{std::forward<ArgTypes>(args)...}
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
                std::apply(wcPtr->function_, wcPtr->args_);
                // std::invoke(wcPtr->function_, 1, 2);
                // wcPtr->function_(1, 2);
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
        std::tuple<ArgTypes...> args_;
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

        template <typename F, typename... ArgTypes>
        WorkImpl createWorkThreadPool(F &&f, ArgTypes &&...args)
        {
            // TODO: ポインタのメモリ管理
            // int *wc = nullptr;
            auto *wc = new WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>{
                std::forward<std::function<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>>(f),
                std::forward<ArgTypes>(args)...};
            PTP_WORK_CALLBACK workcallback = WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>::wrapper;
            PTP_WORK work = NULL;
            work = CreateThreadpoolWork(workcallback, wc, &callBackEnviron_);
            if (NULL == work) {
                throw std::runtime_error{"CreateThreadpoolWork failed. LastError: " + GetLastError()};
            }
            return WorkImpl{work};
        }

    private:
        PTP_POOL pool_;
        TP_CALLBACK_ENVIRON callBackEnviron_;
        PTP_CLEANUP_GROUP cleanupgroup_;
    };

} // namespace Eihire2::Inner

#endif // Eihire2_Inner_ThreadPool_Win32_INCLUDED
