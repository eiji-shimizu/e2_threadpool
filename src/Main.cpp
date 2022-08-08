#include "ThreadPool.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    try {
        Eihire2::ThreadPool threadPool;
        Eihire2::WorkThread t1 = threadPool.addWorkThread();
        t1.start();
        threadPool.addWorkThread().start();
        t1.start();
        t1.start();
        t1.start();
        t1.start();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "unexpected error." << std::endl;
    }
}