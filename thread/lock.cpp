#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <future>

using Seconds = std::chrono::seconds;

struct Locktest {
    std::timed_mutex m;
    int num;

    void read() {
        std::cout << "Try reading" << std::endl;
        if (m.try_lock_for(Seconds{6})) {
            std::lock_guard<std::timed_mutex> lg{m, std::adopt_lock};
            std::cout << "read num: " << num << std::endl;
        } else {
            std::cerr << "timeout reading num " << std::endl;
        }
        std::cout << "End reading num " << std::endl;
    }

    void write() {
        std::cout << "Try writing" << std::endl;
        std::lock_guard<std::timed_mutex> lg{m};
        std::this_thread::sleep_for(Seconds{5});
        num = 34;
        std::cout << "End writing number" << std::endl;
    }
};


int main () {
    Locktest lt;
    auto f1 = std::async(std::launch::async, [&lt](){
        lt.write();
    });

    auto f2 = std::async([&lt](){
        lt.read();
    });
    std::cout << std::thread::hardware_concurrency() << std::endl;
    f2.get();
    f1.get();
}

