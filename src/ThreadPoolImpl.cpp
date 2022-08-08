#include "inner/ThreadPool_Win32.h"

#include <exception>
#include <iostream>

namespace Eihire2::Inner {

    //
    // This is the thread pool work callback function.
    //
    VOID
        CALLBACK
        MyWorkCallback(
            PTP_CALLBACK_INSTANCE Instance,
            PVOID Parameter,
            PTP_WORK Work)
    {
        // Instance, Parameter, and Work not used in this example.
        UNREFERENCED_PARAMETER(Instance);
        UNREFERENCED_PARAMETER(Parameter);
        UNREFERENCED_PARAMETER(Work);

        BOOL bRet = FALSE;

        //
        // Do something when the work callback is invoked.
        //
        {
            std::cout << "MyWorkCallback: Task performed.\n";
        }
        std::cout << "OK.\n";
        return;
    }

    WorkThreadImpl::WorkThreadImpl(PTP_WORK work)
        : work_{work}
    {
        // noop
    }

    WorkThreadImpl::~WorkThreadImpl()
    {
        // noop
    }

    void WorkThreadImpl::submit()
    {
        SubmitThreadpoolWork(work_);
    }

    ThreadPoolImpl::ThreadPoolImpl()
        : pool_{NULL},
          cleanupgroup_{NULL}
    {
        BOOL bRet = FALSE;
        InitializeThreadpoolEnvironment(&callBackEnviron_);
        pool_ = CreateThreadpool(NULL);
        if (NULL == pool_) {
            throw std::runtime_error{"CreateThreadpool failed. LastError: " + GetLastError()};
        }

        SetThreadpoolThreadMaximum(pool_, 3);
        bRet = SetThreadpoolThreadMinimum(pool_, 1);
        if (FALSE == bRet) {
            throw std::runtime_error{"SetThreadpoolThreadMinimum failed. LastError: " + GetLastError()};
        }

        cleanupgroup_ = CreateThreadpoolCleanupGroup();
        if (NULL == cleanupgroup_) {
            throw std::runtime_error{"CreateThreadpoolCleanupGroup failed. LastError: " + GetLastError()};
        }

        SetThreadpoolCallbackPool(&callBackEnviron_, pool_);
        SetThreadpoolCallbackCleanupGroup(&callBackEnviron_, cleanupgroup_, NULL);
    }

    ThreadPoolImpl::~ThreadPoolImpl()
    {
        try {
            std::cout << "ThreadPoolImpl::~ThreadPoolImpl() BEFORE" << std::endl;
            CloseThreadpoolCleanupGroupMembers(cleanupgroup_, FALSE, NULL);
            CloseThreadpoolCleanupGroup(cleanupgroup_);
            CloseThreadpool(pool_);
            std::cout << "ThreadPoolImpl::~ThreadPoolImpl() AFTER" << std::endl;
        }
        catch (...) {
            // 何もできない
        }
    }

    WorkThreadImpl ThreadPoolImpl::createThread()
    {
        PTP_WORK_CALLBACK workcallback = MyWorkCallback;
        PTP_WORK work = NULL;
        work = CreateThreadpoolWork(workcallback, NULL, &callBackEnviron_);
        if (NULL == work) {
            throw std::runtime_error{"CreateThreadpoolWork failed. LastError: " + GetLastError()};
        }
        return WorkThreadImpl{work};
    }

} //  namespace Eihire2::Inner
