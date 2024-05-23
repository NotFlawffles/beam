#pragma once

#include "../../lexer/token.hpp"

namespace Beam::Text::Parser::Expression {
class Operator: IO::Format::Display {
  public:
    enum class Type {
        Plus,
        Minus,
        Multiply,
        Divide,
        Remainder,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        BitwiseNot,
        BitwiseLeftShift,
        BitwiseRightShift,
        LogicalNot,
        Assign,
        NotEqual,
        Equal,
        LogicalAnd,
        LogicalOr,
        LessThan,
        GreaterThan,
        LessThanOrEqual,
        GreaterThanOrEqual,
        PlusAssign,
        MinusAssign,
        MultiplyAssign,
        DivideAssign,
        RemainderAssign,
        BitwiseAndAssign,
        BitwiseOrAssign,
        BitwiseXorAssign,
        BitwiseLeftShiftAssign,
        BitwiseRightShiftAssign,
        Comma,
        Invalid
    };

    Operator(const Type& type, const std::string& value)
        : type(type), value(value) {}

    static Operator* FromToken(const Lexer::Token& token);

    Type getType() const { return type; }

    std::string getValue() const { return value; }

    std::string getTypeAsString() const;

    std::string format() override, debug() override;

  private:
    const Type type;
    const std::string value;
};
} // namespace Beam::Text::Parser::Expression
