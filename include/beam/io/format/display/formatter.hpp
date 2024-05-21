#pragma once

#include <string>

namespace Beam::IO::Format::Display {
class Formatter {
  public:
    virtual std::string format() = 0;
};
} // namespace Beam::IO::Format::Display
