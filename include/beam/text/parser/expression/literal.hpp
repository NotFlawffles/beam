#pragma once

#include "expression.hpp"

namespace Beam::Text::Parser::Expression {
class Literal: public Expression {
  public:
    enum class Type { Name, Integer, Float, Character, String };

    Literal(const Type& literalType, const std::string& nameValue,
            const std::string& stringValue, const std::size_t& integerValue,
            const float& floatValue, const char& characterValue)
        : Expression(Expression::Type::Literal), literalType(literalType),
          nameValue(nameValue), stringValue(stringValue),
          integerValue(integerValue), floatValue(floatValue),
          characterValue(characterValue) {}

    static Literal Name(const std::string& value) {
        return Literal(Type::Name, value, value, value.length(), value.length(),
                       value.at(0));
    }

    static Literal Integer(const std::size_t& value) {
        return Literal(Type::Integer, "integer", std::to_string(value), value,
                       value, value);
    }

    static Literal Float(const float& value) {
        return Literal(Type::Float, "float", std::to_string(value), value,
                       value, value);
    }

    static Literal Character(const char& value) {
        return Literal(Type::Character, "character", std::to_string(value),
                       value, value, value);
    }

    static Literal String(const std::string& value) {
        return Literal(Type::String, value, value, value.length(),
                       value.length(), value.at(0));
    }

    std::string getLiteralTypeAsString() const, format() override,
        debug() override;

  private:
    const Type literalType;

    std::string nameValue;
    std::string stringValue;
    std::size_t integerValue;
    float floatValue;
    char characterValue;
};
} // namespace Beam::Text::Parser::Expression
