#pragma once

#include "../display/debugger.hpp"
#include "../display/formatter.hpp"
#include <string>

namespace Beam::Debug::Types {
class Char: IO::Format::Display::Formatter, IO::Format::Display::Debugger {
  public:
    Char(const char& value): value(value) {}

    std::string format() override { return std::string({value}); }

    std::string debug() override {
        return "DisplyableChar(\'" + std::string({value}) + "\')";
    }

  private:
    char value;
};
} // namespace Beam::Debug::Types
