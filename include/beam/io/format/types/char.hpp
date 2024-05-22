#pragma once

#include "../display.hpp"
#include <string>

namespace Beam::IO::Format::Types {
class Char: Display {
  public:
    Char(const char& value): value(value) {}

    std::string format() override { return std::string({value}); }

    std::string debug() override {
        return "Char(value: \'" + std::string({value}) + "\')";
    }

  private:
    char value;
};
} // namespace Beam::IO::Format::Types
