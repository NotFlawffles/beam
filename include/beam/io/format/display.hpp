#pragma once

#include <string>

namespace Beam::IO::Format {
class Display {
  public:
    virtual std::string format() = 0, debug() = 0;
};
} // namespace Beam::IO::Format
