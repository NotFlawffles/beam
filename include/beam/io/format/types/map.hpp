#pragma once

#include <initializer_list>
#include <iostream>
#include <map>

#include "../display.hpp"
#include "../utils/typename.hpp"

namespace Beam::IO::Format::Types {
template<typename K, typename V,
         typename = std::enable_if_t<
             std::is_pointer<K>::value &&
             std::is_base_of<Display, std::remove_pointer_t<K>>::value &&
             std::is_pointer<V>::value &&
             std::is_base_of<Display, std::remove_pointer_t<V>>::value>>
class Map: Display, public std::map<K, V> {
  public:
    explicit Map(const std::map<K, V>& value) {
        for (const auto& [ckey, cvalue] : value) {
            this->insert(std::pair<K, V>(ckey, cvalue));
        }
    }

    bool has(const K& key) const {
        for (const auto& [ckey, cvalue] : *this) {
            if (*key == *ckey) {
                return true;
            }
        }

        return false;
    }

    V get(const K& key) const {
        for (const auto& [ckey, cvalue] : *this) {
            if (*key == *ckey) {
                return cvalue;
            }
        }

        return nullptr;
    }

    std::string format() override {
        auto content = std::string("{");

        for (const auto& [key, value] : *this) {
            content.append(key->format() + ": " + value->format());
        }

        content.push_back('}');
        return content;
    }

    std::string debug() override {
        return "Map(value: std::map<" + Utils::Typename::Get<K>() + ", " +
               Utils::Typename::Get<V>() + ">(" + format() + ")";
    }
};
} // namespace Beam::IO::Format::Types
