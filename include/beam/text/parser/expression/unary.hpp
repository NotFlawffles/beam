#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"
#include "operator.hpp"

namespace Beam::Text::Parser::Expression {
class Unary: public Expression {
  public:
    Unary(Expression* expression, Operator* _operator,
          const IO::String::Span& operatorSpan,
          const IO::String::Span& expressionSpan)
        : Expression(Expression::Type::Unary), expression(expression),
          _operator(_operator), expressionSpan(expressionSpan),
          operatorSpan(operatorSpan) {}

    Expression* getExpression() const { return expression; }

    Operator* getOperator() const { return _operator; }

    IO::String::Span getExpressionSpan() const { return expressionSpan; }

    IO::String::Span getOperatorSpan() const { return operatorSpan; }

    std::string format() override, debug() override;

  private:
    Expression* expression;
    Operator* _operator;

    const IO::String::Span expressionSpan, operatorSpan;
};
} // namespace Beam::Text::Parser::Expression
