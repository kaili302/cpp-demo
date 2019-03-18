#include <chrono>
#include <mutex>
#include <iostream>
#include <future>
#include "cat.h"
#include "print.h"

struct a {
void test() {
    auto f1 = std::async(std::launch::async, [](){
        std::cout << stdstr::cat(3, 3, 3, 3, 3, 3, 3, 3, 3, 3) << std::endl;
    });

    auto f2 = std::async(std::launch::async, [](){
        std::cout << stdstr::cat(4, 4, 4, 4, 4, 4, 4, 4, 4, 4) << std::endl;
    });
}
};

int main() {
    //a().test();
    auto f1 = std::async(std::launch::async, [](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << stdstr::cat(3, 3, 3, 3, 3, 3, 3, 3, 3, 3) << std::endl;
    });

    auto f2 = std::async(std::launch::async, [](){
        std::cout << stdstr::cat(4, 4, 4, 4, 4, 4, 4, 4, 4, 4) << std::endl;
    });
}
