#pragma once

#include <iostream>
#include <type_traits>

#include "../io/format/display/debugger.hpp"
#include "error.hpp"

namespace Beam::Diagnostic {
template<typename V, typename = std::enable_if_t<std::is_base_of<
                         IO::Format::Display::Debugger, V>::value>>
class Result: IO::Format::Display::Debugger {
  public:
    Result(const V& value): value(value), hasValue(true) {}

    Result(const Error& error): error(error), hasValue(false) {}

    bool isSuccess() const { return hasValue; }

    bool isFailure() const { return !hasValue; }

    V getValue() const { return value; }

    Error getError() const { return error; }

    std::string debug() override {
        auto content = std::string();

        if (isSuccess()) {
            content.append("Success(" + getValue().debug());
        } else {
            content.append("Failure(" + getError().debug());
        }

        content.push_back(')');
        return content;
    };

  private:
    V value;
    Error error;
    bool hasValue;
};
} // namespace Beam::Diagnostic
