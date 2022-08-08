#include "ThreadPool.h"

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main()
{
    try {
        Eihire2::ThreadPool threadPool;
        std::function<int(int, int)> f = [](int a, int b) { 
            int c = a + b;
            std::cout << "Hello World." + std::to_string(c) << std::endl;
            return a + b; };

        auto copy{f};

        Eihire2::Work w1 = threadPool.addWork(std::move(f), 1, 2);
        w1.start();

        threadPool.addWork([](int a, int b) { 
            int c = a + b;
            std::cout << "Hello World." + std::to_string(c) << std::endl;
            return a + b; }, 3, 5)
            .start();

        w1.start();
        w1.start();
        w1.start();
        w1.start();

        Eihire2::Work w3 = threadPool.addWork(std::move(copy), 99, 1);
        w3.start();
        w3.start();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "unexpected error." << std::endl;
    }
}