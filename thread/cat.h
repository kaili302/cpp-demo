#include <iostream>
#include <sstream>
#include <string>

namespace stdstr {
namespace detail {
class Cat {
  public:
    template <typename T, typename... ARGS>
    void operator()(std::ostream& os, T&& t, ARGS&& ...args) {
        os << t;
        operator()(os, std::forward<ARGS>(args)...);
    }

  private:
    void operator()(std::ostream&) {}
};
} // close namespace detail

template <typename... ARGS>
std::string cat(ARGS&& ...args) {
    std::ostringstream oss;
    detail::Cat()(oss, std::forward<ARGS>(args)...);
    return oss.str();
}

} // close namespace stdstr
