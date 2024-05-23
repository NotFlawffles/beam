#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"
#include "operator.hpp"

namespace Beam::Text::Parser::Expression {
class Unary: public Expression {
  public:
    Unary(Expression* expression, Operator* _operator)
        : Expression(Expression::Type::Unary), expression(expression),
          _operator(_operator) {}

    Expression* getExpression() const { return expression; }

    Operator* getOperator() const { return _operator; }

    std::string format() override, debug() override;

  private:
    Expression* expression;
    Operator* _operator;
};
} // namespace Beam::Text::Parser::Expression
