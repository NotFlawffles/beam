#pragma once

#include <iostream>
#include <type_traits>

#include "../io/format/display.hpp"

namespace Beam::Diagnostic {
template<
    typename E,
    typename = std::enable_if_t<
        std::is_pointer<E>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<E>>::value>>
class MonoResult: IO::Format::Display {
  public:
    MonoResult(const E& error): error(error), hasError(true) {}

    MonoResult(): hasError(false) {}

    bool isSuccess() const { return !hasError; }

    bool isFailure() const { return hasError; }

    E getError() const { return error; }

    std::string format() override {
        return isSuccess() ? "" : getError()->format();
    };

    std::string debug() override {
        auto content = std::string("MonoResult(");

        if (isSuccess()) {
            content.append("Success(");
        } else {
            content.append("Failure(error: " + getError()->debug());
        }

        content.append("))");
        return content;
    };

  private:
    E error;
    bool hasError;
};

template<
    typename V, typename E,
    typename = std::enable_if_t<
        std::is_pointer<V>::value && std::is_pointer<E>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<V>>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<E>>::value>>
class DiResult: IO::Format::Display {
  public:
    DiResult(const V& value): value(value), hasValue(true) {}

    DiResult(const E& error): error(error), hasValue(false) {}

    DiResult(): hasValue(false) {}

    bool isSuccess() const { return hasValue; }

    bool isFailure() const { return !hasValue; }

    V getValue() const { return value; }

    E getError() const { return error; }

    std::string format() override {
        return isSuccess() ? getValue()->format() : getError()->format();
    };

    std::string debug() override {
        auto content = std::string("DiResult(");

        if (isSuccess()) {
            content.append("Success(value: " + getValue()->debug());
        } else {
            content.append("Failure(error: " + getError()->debug());
        }

        content.append("))");
        return content;
    };

  private:
    V value;
    E error;
    bool hasValue;
};
} // namespace Beam::Diagnostic
