#include <chrono>
#include <future>
#include <mutex>
#include <queue>
#include "../util/print.h"

class System {
  public:
    template <size_t N>
    void publish(int startFrom) {
        for (size_t i = 0; i < N; ++i) {
            {
                std::lock_guard<std::mutex> lg{d_qMutex};
                d_dataQ.push(startFrom + i);
                stdstr::print("thread[", std::this_thread::get_id(),
                              "] publish data: ", startFrom + i);
            }
            d_qCondVar.notify_one(); // consume mode
            // d_qCondVar.notify_all(); // subscribe mode

            std::this_thread::sleep_for(std::chrono::seconds{2});
        }
    }

    void consume() {
        while(true) {
            int val;
            {
                std::unique_lock<std::mutex> ul{d_qMutex};
                d_qCondVar.wait(ul, [this]{ return !d_dataQ.empty(); });
                val = d_dataQ.front();
                d_dataQ.pop();
            }
            stdstr::print("thread[", std::this_thread::get_id(),
                          "] consume data: ", val);
        }
    }

  private:
    std::mutex d_qMutex;
    std::condition_variable d_qCondVar;
    std::queue<int> d_dataQ;
};

int main() {
    System sys;

    using Jobs = std::vector<std::future<void>>;
    Jobs pubs;
    constexpr size_t numPub = 4;
    for (size_t i = 0; i < numPub; ++i) {
        pubs.emplace_back(std::async(std::launch::async, [&sys, i]{
            sys.publish<10>(i*10);
        }));
    }

    Jobs cons;
    constexpr size_t numCon = 10;
    for (size_t i = 0; i < numCon; ++i) {
        cons.emplace_back(std::async(std::launch::async, [&sys]{
            sys.consume();
        }));
    }
}
