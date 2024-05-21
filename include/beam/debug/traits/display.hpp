#pragma once

#include <string>

namespace Beam::Debug::Traits {
class Display {
  public:
    virtual std::string debug() = 0;
};
} // namespace Beam::Debug::Traits
