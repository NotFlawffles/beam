#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"

namespace Beam::Text::Parser::Expression {
class Unary: public Expression {
  public:
    Unary(Expression* expression, const Lexer::Token& _operator)
        : Expression(Expression::Type::Unary), expression(expression),
          _operator(_operator) {}

    Expression* getExpression() const { return expression; }

    Lexer::Token getOperator() const { return _operator; }

    std::string format() override, debug() override;

  private:
    Expression* expression;
    Lexer::Token _operator;
};
} // namespace Beam::Text::Parser::Expression
