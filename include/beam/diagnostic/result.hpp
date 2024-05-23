#pragma once

#include <iostream>
#include <type_traits>

#include "../io/format/display.hpp"
#include "error.hpp"

namespace Beam::Diagnostic {
template<
    typename V, typename E,
    typename = std::enable_if_t<
        std::is_pointer<V>::value && std::is_pointer<E>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<V>>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<E>>::value>>
class Result: IO::Format::Display {
  public:
    Result(const V& value): value(value), hasValue(true) {}

    Result(const E& error): error(error), hasValue(false) {}

    Result(): hasValue(false) {}

    bool isSuccess() const { return hasValue; }

    bool isFailure() const { return !hasValue; }

    V getValue() const { return value; }

    E getError() const { return error; }

    std::string format() override {
        return isSuccess() ? getValue()->format() : getError()->format();
    };

    std::string debug() override {
        auto content = std::string();

        if (isSuccess()) {
            content.append("Success(value: " + getValue()->debug());
        } else {
            content.append("Failure(error: " + getError()->debug());
        }

        content.push_back(')');
        return content;
    };

  private:
    V value;
    E error;
    bool hasValue;
};
} // namespace Beam::Diagnostic
