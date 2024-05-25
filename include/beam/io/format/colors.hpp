#pragma once

#include "../../diagnostic/error.hpp"
#include "../../diagnostic/result.hpp"
#include "display.hpp"
#include "types/map.hpp"
#include "types/string.hpp"
#include "types/vector.hpp"

namespace Beam::IO::Format::Colors {
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

class Colorscheme: Display {
  public:
    Colorscheme(const Types::Map<Types::String*, Color*>& colors)
        : colors(colors) {}

    Diagnostic::DiResult<Types::String*, Diagnostic::Error*>
    color(const std::string& target) const;

    std::string format() override { return colors.format(); }

    std::string debug() override {
        return "Colorscheme(colors: " + colors.debug() + ')';
    }

  private:
    Types::Map<Types::String*, Color*> colors;
};
} // namespace Beam::IO::Format::Colors
