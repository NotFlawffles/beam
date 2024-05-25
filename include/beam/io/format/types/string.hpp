#pragma once

#include <iostream>
#include <string>

#include "../display.hpp"

namespace Beam::IO::Format::Types {
class String: Display, public std::string {
  public:
    explicit String(const std::string& value) { this->assign(value); }

    std::string get() const { return *this; }

    std::string format() override { return *this; }

    std::string debug() override {
        return "String(value: std::string(\"" + *this + "\")";
    }
};
} // namespace Beam::IO::Format::Types
