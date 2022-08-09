#include "inner/ThreadPool_Win32.h"

#include <any>
#include <exception>
#include <iostream>
#include <vector>

namespace Eihire2::Inner {

    /**
     *  WorkImpl
     */
    WorkImpl::WorkImpl(PTP_WORK work, std::shared_ptr<void> workCallbackPtr)
        : work_{work},
          workCallbackPtr_{workCallbackPtr}
    {
        // noop
    }

    WorkImpl::~WorkImpl()
    {
        std::cout << "~WorkImpl()" << std::endl;
        WaitForThreadpoolWorkCallbacks(work_, true);
    }

    void WorkImpl::submit()
    {
        SubmitThreadpoolWork(work_);
    }

    /**
     *  ThreadPoolImpl
     */
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
            CloseThreadpoolCleanupGroupMembers(cleanupgroup_, true, NULL);
            CloseThreadpoolCleanupGroup(cleanupgroup_);
            CloseThreadpool(pool_);
            std::cout << "ThreadPoolImpl::~ThreadPoolImpl() AFTER" << std::endl;
        }
        catch (...) {
            // 何もできない
        }
    }

} //  namespace Eihire2::Inner
