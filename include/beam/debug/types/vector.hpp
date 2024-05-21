#pragma once

#include <type_traits>
#include <vector>

#include "../traits/display.hpp"

namespace Beam::Debug::Types {
template<typename T,
         typename =
             std::enable_if_t<std::is_base_of<Traits::Display, T>::value>>
class Vector: public std::vector<T>, Traits::Display {
  public:
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
