#pragma once

#include <vector>

#include "../display.hpp"
#include "../utils/typename.hpp"

#define Vec(type, value) Beam::IO::Format::Types::Vector<type>(value)

namespace Beam::IO::Format::Types {
template<typename T,
         typename = std::enable_if_t<
             std::is_pointer<T>::value &&
             std::is_base_of<Display, std::remove_pointer_t<T>>::value>>
class Vector: public std::vector<T>, Display {
  public:
    explicit Vector(const std::vector<T>& value): std::vector<T>(value) {}

    std::string formatNoBraces() {
        auto result = format();
        result.erase(0, 1);
        result.erase(result.length() - 1);
        return result;
    }

    std::string format() override {
        auto content = std::string("{");

        for (std::size_t index = 0; index < this->size(); index++) {
            content.append(this->at(index)->format());

            if (index + 1 < this->size()) {
                content.append(", ");
            }
        }

        content.push_back('}');
        return content;
    }

    std::string debug() override {
        auto content = std::string("Vector(value: std::vector<" +
                                   Utils::Typename::Get<T>() + ">({");

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
} // namespace Beam::IO::Format::Types
