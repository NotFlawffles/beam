#pragma once

#include "../../../diagnostic/error.hpp"
#include "../../../diagnostic/result.hpp"
#include "../display.hpp"
#include "../types/map.hpp"
#include "../types/string.hpp"
#include "color.hpp"

namespace Beam::IO::Format::Color {
class Colorscheme: Display {
  public:
    Colorscheme(const Types::Map<Types::String*, Color*>& colors)
        : colors(colors) {}

    Diagnostic::DiResult<Types::String*, Diagnostic::Diagnostic*>
    color(const std::string& target) const,
        color(const std::string& target, const std::string& which,
              String::Span span) const;

    std::string format() override { return colors.format(); }

    std::string debug() override {
        return "Colorscheme(colors: " + colors.debug() + ')';
    }

  private:
    Types::Map<Types::String*, Color*> colors;
};
} // namespace Beam::IO::Format::Color
