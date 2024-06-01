#pragma once

#include <iostream>
#include <string>

#include "../../string/span.hpp"
#include "../display.hpp"

namespace Beam::IO::Format::Types {
class String: Display, public std::string {
  public:
    String(const std::string& value) { this->assign(value); }

    String(const std::string& value, const IO::String::Span& span): span(span) {
        this->assign(value);
    }

    std::string get() const { return *this; }

    IO::String::Span getSpan() const { return span; }

    std::string format() override { return *this; }

    std::string debug() override {
        return "String(value: std::string(\"" + *this + "\")";
    }

  private:
    const IO::String::Span span;
};
} // namespace Beam::IO::Format::Types
