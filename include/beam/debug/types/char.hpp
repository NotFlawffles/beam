#pragma once

#include "../traits/display.hpp"
#include <string>

namespace Beam::Debug::Types {
class Char: Traits::Display {
  public:
    Char(const char& value): value(value) {}

    std::string debug() override {
        return "DisplyableChar(\'" + std::string({value}) + "\')";
    }

  private:
    char value;
};
} // namespace Beam::Debug::Types
