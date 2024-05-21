#pragma once

#include <type_traits>
#include <vector>

#include "../display/debugger.hpp"
#include "../display/formatter.hpp"

namespace Beam::Debug::Types {
template<typename T, typename = std::enable_if_t<std::is_base_of<
                         IO::Format::Display::Debugger, T>::value>>
class Vector: public std::vector<T>,
              IO::Format::Display::Formatter,
              IO::Format::Display::Debugger {
  public:
    std::string format() override {
        auto content = std::string("{");

        for (std::size_t index = 0; index < this->size(); index++) {
            content.append(this->at(index).debug());

            if (index + 1 < this->size()) {
                content.append(", ");
            }
        }

        content.push_back('}');
        return content;
    }

    std::string debug() override {
        auto content = std::string("DisplayableVector({");

        for (std::size_t index = 0; index < this->size(); index++) {
            content.append(this->at(index).debug());

            if (index + 1 < this->size()) {
                content.append(", ");
            }
        }

        content.append("})");
        return content;
    }
};
} // namespace Beam::Debug::Types
