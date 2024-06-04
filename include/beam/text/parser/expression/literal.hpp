#pragma once

#include "../../../io/format/types/vector.hpp"
#include "expression.hpp"

namespace Beam::Text::Parser::Expression {
class Literal: public Expression {
  public:
    enum class Type { Name, FunctionCall, Integer, Float, Character, String };

    Literal(const Type& literalType, const std::string& nameValue,
            const std::string& stringValue, const std::size_t& integerValue,
            const float& floatValue, const char& characterValue,
            IO::Format::Types::Vector<Expression*>* arguments,
            const IO::String::Span& span)
        : Expression(Expression::Type::Literal), literalType(literalType),
          nameValue(nameValue), stringValue(stringValue),
          integerValue(integerValue), floatValue(floatValue),
          characterValue(characterValue), arguments(arguments), span(span) {}

    static Literal* Name(const std::string& value,
                         const IO::String::Span& span) {
        return new Literal(Type::Name, value, value, value.length(),
                           value.length(), value.at(0), {}, span);
    }

    static Literal*
    FunctionCall(const std::string& value,
                 IO::Format::Types::Vector<Expression*>* arguments,
                 const IO::String::Span& span) {
        return new Literal(Type::FunctionCall, value, value, value.length(),
                           value.length(), value.at(0), arguments, span);
    }

    static Literal* Integer(const std::size_t& value,
                            const IO::String::Span& span) {
        return new Literal(Type::Integer, "integer", std::to_string(value),
                           value, value, value, {}, span);
    }

    static Literal* Float(const float& value, const IO::String::Span& span) {
        return new Literal(Type::Float, "float", std::to_string(value), value,
                           value, value, {}, span);
    }

    static Literal* Character(const char& value, const IO::String::Span& span) {
        return new Literal(Type::Character, "character", std::to_string(value),
                           value, value, value, {}, span);
    }

    static Literal* String(const std::string& value,
                           const IO::String::Span& span) {
        return new Literal(Type::String, value, value, value.length(),
                           value.length(), value.at(0), {}, span);
    }

    Type getLiteralType() const { return literalType; }

    IO::String::Span getSpan() const { return span; }

    std::string getLiteralTypeAsString() const, format() override,
        debug() override;

    std::string getNameValue() const { return nameValue; }

    std::string getStringValue() const { return stringValue; }

    std::size_t getIntegerValue() const { return integerValue; }

    float getFloatValue() const { return floatValue; }

    char getCharacterValue() const { return characterValue; }

    IO::Format::Types::Vector<Expression*>* getArguments() const {
        return arguments;
    }

  private:
    const Type literalType;

    std::string nameValue;
    std::string stringValue;
    std::size_t integerValue;
    float floatValue;
    char characterValue;
    IO::Format::Types::Vector<Expression*>* arguments;

    const IO::String::Span span;
};
} // namespace Beam::Text::Parser::Expression
