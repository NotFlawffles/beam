#pragma once

#include "../display.hpp"

namespace Beam::IO::Format::Color {
class Color: Display {
  public:
    enum Type : char {
        ColorTypeReset = 0,
        ColorTypeBlack = 30,
        ColorTypeRed,
        ColorTypeGreen,
        ColorTypeYellow,
        ColorTypeBlue,
        ColorTypeMagenta,
        ColorTypeCyan,
        ColorTypeWhite
    };

    explicit Color(const Type& value)
        : value("[" + std::to_string(value) + "m") {}

    std::string get() const { return value; }

    std::string format() override { return value; }

    std::string debug() override { return "Color(" + get() + ')'; }

  private:
    std::string value;
};
} // namespace Beam::IO::Format::Color
