#pragma once

#include <string>

namespace Beam::IO {
class Display {
  public:
    virtual std::string debug() = 0;
};
} // namespace Beam::IO
