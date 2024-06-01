#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"
#include "operator.hpp"

namespace Beam::Text::Parser::Expression {
class Binary: public Expression {
  public:
    Binary(Expression* left, Operator* _operator, Expression* right,
           const IO::String::Span& leftSpan,
           const IO::String::Span& operatorSpan,
           const IO::String::Span& rightSpan)
        : Expression(Expression::Type::Binary), left(left), right(right),
          _operator(_operator), leftSpan(leftSpan), rightSpan(rightSpan),
          operatorSpan(operatorSpan) {}

    Expression* getLeft() const { return left; };

    Expression* getRight() const { return right; };

    Operator* getOperator() const { return _operator; }

    IO::String::Span getLeftSpan() const { return leftSpan; }

    IO::String::Span getRightSpan() const { return rightSpan; }

    IO::String::Span getOperatorSpan() const { return operatorSpan; }

    std::string format() override, debug() override;

  private:
    Expression *left, *right;
    Operator* _operator;

    const IO::String::Span leftSpan, rightSpan, operatorSpan;
};
} // namespace Beam::Text::Parser::Expression
