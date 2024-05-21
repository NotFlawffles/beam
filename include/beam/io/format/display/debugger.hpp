#pragma once

#include <string>

namespace Beam::IO::Format::Display {
class Debugger {
  public:
    virtual std::string debug() = 0;
};
} // namespace Beam::IO::Format::Display
