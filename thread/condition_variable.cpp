#include <mutex>
#include <chrono>
#include <iostream>
#include <future>
#include <condition_variable>

struct Condt {
    int sharedValue;
    bool isReady;
    std::mutex readyMutex;
    std::condition_variable readyCondVar;

    Condt() : isReady{false} {}

    void job1() {
        std::cout << "start job1 \n";
        std::this_thread::sleep_for(std::chrono::seconds{5});

        {
            std::lock_guard<std::mutex> lg{readyMutex};
            sharedValue = 100;
            isReady = true;
        }
        readyCondVar.notify_all();
    }

    void job2() {
        std::cout << "start job2 \n";

        std::unique_lock<std::mutex> ul{readyMutex};

        readyCondVar.wait(ul, [this]{ return isReady; });

        std::cout << "job2: continue processing with shared value: "
                  << sharedValue << std::endl;
    }

    void job3() {
        std::cout << "start job3 \n";

        std::unique_lock<std::mutex> ul{readyMutex};

        while(!isReady) {
            // might wake up even is not ready
            readyCondVar.wait(ul);
        }
        std::cout << "job3: continue processing with shared value: "
                  << sharedValue << std::endl;
    }

    void job4() {
        std::cout << "start job4 \n";

        std::unique_lock<std::mutex> ul{readyMutex};

        while(!isReady) {
            ul.unlock();
            std::this_thread::yield();
            ul.lock();
        }
        std::cout << "job4: continue processing with shared value: "
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

    auto f3 = std::async(std::launch::async, [&condTest](){
        condTest.job3();
    });

    auto f4 = std::async(std::launch::async, [&condTest](){
        condTest.job4();
    });
}
