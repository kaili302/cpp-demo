#include <future>
#include <iostream>
#include <mutex>

namespace stdstr {
namespace detail {
class Print {
  public:
   template <typename T, typename... ARGS>
    void operator()(T&& t, ARGS&& ...args) {
        std::lock_guard<std::recursive_mutex> g{d_rm};
        std::cout << t;
        operator()(std::forward<ARGS>(args)...);
    };

    void operator()(){
        std::lock_guard<std::recursive_mutex> g{d_rm};
        std::cout << std::endl;
    }

   private:
    std::recursive_mutex d_rm;
};
} // close namespace detail

template <typename... ARGS>
void print(ARGS&& ...args) {
    detail::Print()(std::forward<ARGS>(args)...);
}

} // close namespace stdstr
