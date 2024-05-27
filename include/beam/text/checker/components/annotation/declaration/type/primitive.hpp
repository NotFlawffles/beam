#pragma once

#include "type.hpp"

namespace Beam::Text::Checker::Components::Annotation::Declaration::Type {
class Primitive: public Type {
  public:
    enum class Type {
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Sint8,
        Sint16,
        Sint32,
        Sint64,

        Char
    };

    Primitive(const std::string& primitiveType,
              const unsigned long long int& depth)
        : Declaration::Type::Type(Declaration::Type::Type::Kind::Primitive,
                                  depth) {
        if (primitiveType == "uint8") {
            this->primitiveType = Type::Uint8;
        } else if (primitiveType == "uint16") {
            this->primitiveType = Type::Uint16;
        } else if (primitiveType == "uint32") {
            this->primitiveType = Type::Uint32;
        } else if (primitiveType == "uint64") {
            this->primitiveType = Type::Uint64;
        } else if (primitiveType == "sint8") {
            this->primitiveType = Type::Sint8;
        } else if (primitiveType == "sint16") {
            this->primitiveType = Type::Sint16;
        } else if (primitiveType == "sint32") {
            this->primitiveType = Type::Sint32;
        } else if (primitiveType == "sint64") {
            this->primitiveType = Type::Sint64;
        } else if (primitiveType == "char") {
            this->primitiveType = Type::Char;
        } else if (primitiveType == "uint") {
            this->primitiveType = Type::Uint32;
        } else if (primitiveType == "sint" || primitiveType == "int") {
            this->primitiveType = Type::Sint32;
        }
    }

    Type getPrimitiveType() const { return primitiveType; }

    std::string getPrimitiveTypeAsString() const, format() override,
        debug() override;

  private:
    Type primitiveType;
};
} // namespace Beam::Text::Checker::Components::Annotation::Declaration::Type
