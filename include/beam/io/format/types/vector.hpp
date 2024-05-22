#pragma once

#include <type_traits>
#include <typeinfo>
#include <vector>

#include "../display.hpp"

namespace Beam::Debug::Types {
template<
    typename T,
    typename = std::enable_if_t<
        std::is_pointer<T>::value &&
        std::is_base_of<IO::Format::Display, std::remove_pointer_t<T>>::value>>
class Vector: public std::vector<T>, IO::Format::Display {
  public:
    Vector(const std::vector<T>& value): std::vector<T>(value) {}

    std::string format() override {
        auto content = std::string("{");

        for (std::size_t index = 0; index < this->size(); index++) {
            content.append(this->at(index)->debug());

            if (index + 1 < this->size()) {
                content.append(", ");
            }
        }

        content.push_back('}');
        return content;
    }

    std::string debug() override {
        std::string className = typeid(T).name();
        size_t start = className.find_last_of("0123456789") + 1;
        size_t end = className.length();

        className = className.substr(start, (end - start) - 1);

        auto content =
            std::string("Vector(value: std::vector<" + className + ">({");

        for (std::size_t index = 0; index < this->size(); index++) {
            content.append(this->at(index)->debug());

            if (index + 1 < this->size()) {
                content.append(", ");
            }
        }

        content.append("})");
        return content;
    }
};
} // namespace Beam::Debug::Types
