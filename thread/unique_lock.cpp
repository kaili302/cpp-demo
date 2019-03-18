#include <mutex>
#include <chrono>
#include <iostream>
#include <future>

struct Condt {
    std::mutex readyMutex;
    bool isReady;
    int sharedValue;

    void job1() {
        std::cout << "start job1 \n";
        std::this_thread::sleep_for(std::chrono::seconds{5});

        std::lock_guard<std::mutex> lg{readyMutex};
        isReady = true;
        sharedValue = 100;
    }

    void job2() {
        std::cout << "start job2 \n";

        std::unique_lock<std::mutex> ul{readyMutex};
        while (!isReady) {
            ul.unlock();
            std::this_thread::yield();
            ul.lock();
        }
        std::cout << "job2: continue processing with shared value: "
                  << sharedValue << std::endl;
    }
};

int main() {
    Condt condTest;
    auto f1 = std::async(std::launch::async, [&condTest](){
        condTest.job1();
    });

    auto f2 = std::async(std::launch::async, [&condTest](){
        condTest.job2();
    });
}
