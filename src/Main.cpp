#include "ThreadPool.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    try {
        Eihire2::ThreadPool threadPool;
        Eihire2::Work w1 = threadPool.addWork();
        w1.start();
        threadPool.addWork().start();
        w1.start();
        w1.start();
        w1.start();
        w1.start();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "unexpected error." << std::endl;
    }
}