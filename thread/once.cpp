#include <mutex>
#include <iostream>
#include <chrono>
#include <vector>
#include <future>

struct A {
    A() {
        std::cout << 1 << std::endl;
    }
};

void test() {
    static std::once_flag of;
    static A a;
    std::call_once(of, [](){
        a = A();
    });
    std::this_thread::sleep_for(std::chrono::seconds{2});
}

int main() {
    std::vector<std::future<void>> fs;

    for (int i = 0; i < 10; ++i) {
        fs.emplace_back(std::async(std::launch::async, test));
    }
}
