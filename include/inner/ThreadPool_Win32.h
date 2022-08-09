#ifndef Eihire2_Inner_ThreadPool_Win32_INCLUDED
#define Eihire2_Inner_ThreadPool_Win32_INCLUDED

#include <windows.h>

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Eihire2::Inner {

    template <typename T = void>
    struct WorkCallbackDeleter {

        WorkCallbackDeleter(PTP_WORK work) noexcept : work_{work} {}

        template <
            typename U,
            typename std::enable_if<std::is_convertible<U *, T *>::value, std::nullptr_t>::type = nullptr>
        WorkCallbackDeleter(const WorkCallbackDeleter<U> &) noexcept {}

        void operator()(T *ptr) const
        {
            std::cout << "delete 1" << std::endl;
            WaitForThreadpoolWorkCallbacks(work_, true);
            delete ptr;
            CloseThreadpoolWork(work_);
            std::cout << "delete 2" << std::endl;
        }

    private:
        PTP_WORK work_;
    };

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

        // ~WorkCallback() = default;
        ~WorkCallback()
        {
            std::cout << "~WorkCallback()" << std::endl;
        }

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
        WorkImpl(PTP_WORK work, std::shared_ptr<void> workCallbackPtr);
        ~WorkImpl();

        void submit();

    private:
        PTP_WORK work_;
        std::shared_ptr<void> workCallbackPtr_;
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
            std::unique_ptr<WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>> up{
                new WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>{
                    std::forward<std::function<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>>(f),
                    std::forward<ArgTypes>(args)...}};

            PTP_WORK_CALLBACK callback = WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>::wrapper;
            PTP_WORK work = NULL;
            work = CreateThreadpoolWork(callback, up.get(), &callBackEnviron_);
            if (NULL == work) {
                throw std::runtime_error{"CreateThreadpoolWork failed. LastError: " + GetLastError()};
            }
            std::shared_ptr<void> p{
                up.get(),
                WorkCallbackDeleter<WorkCallback<std::invoke_result_t<F, ArgTypes...>(ArgTypes...)>>{work}};
            up.release();
            return WorkImpl{work, p};
        }

    private:
        PTP_POOL pool_;
        TP_CALLBACK_ENVIRON callBackEnviron_;
        PTP_CLEANUP_GROUP cleanupgroup_;
    };

} // namespace Eihire2::Inner

#endif // Eihire2_Inner_ThreadPool_Win32_INCLUDED
