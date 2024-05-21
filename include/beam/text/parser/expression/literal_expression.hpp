#pragma once

#include "../../lexer/token.hpp"

namespace Beam::Text::Parser::Expression {
class Expression;

class LiteralExpression {
  public:
    enum Type {
        LiteralExpressionTypeName,
        LiteralExpressionTypeInteger,
        LiteralExpressionTypeFloat,
        LiteralExpressionTypeCharacter,
        LiteralExpressionTypeString
    };

    LiteralExpression(const std::string& value, bool isString)
        : type(isString ? LiteralExpressionTypeString
                        : LiteralExpressionTypeName) {
        isString ? stringValue.assign(value) : nameValue.assign(value);
    }

    explicit LiteralExpression(const std::size_t& value)
        : type(LiteralExpressionTypeInteger), integerValue(value) {}

    explicit LiteralExpression(const float& value)
        : type(LiteralExpressionTypeFloat), floatValue(value) {}

    explicit LiteralExpression(const char& value)
        : type(LiteralExpressionTypeCharacter), characterValue(value) {}

    template<typename T> T getValue() const;
    std::string getTypeAsString() const;

  private:
    const Type type;

    std::string nameValue;
    std::size_t integerValue;
    float floatValue;
    char characterValue;
    std::string stringValue;
};
} // namespace Beam::Text::Parser::Expression
