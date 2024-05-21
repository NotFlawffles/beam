#pragma once

#include "../../io/display.hpp"
#include <string>

namespace Beam::Text::Debug {
class DisplayableChar: Beam::IO::Display {
  public:
    DisplayableChar(const char& value): value(value) {}

    std::string debug() override {
        return "DisplyableChar(\'" + std::string({value}) + "\')";
    }

  private:
    char value;
};
} // namespace Beam::Text::Debug
