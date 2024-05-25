#pragma once

#include <string>
#include <typeinfo>

namespace Beam::IO::Format::Utils {
class Typename {
  public:
    template<typename T> static std::string Get() {
        std::string className = typeid(T).name();
        size_t start = className.find_last_of("0123456789") + 1;
        size_t end = className.length();
        return className = className.substr(start, (end - start) - 1);
    }
};
} // namespace Beam::IO::Format::Utils
