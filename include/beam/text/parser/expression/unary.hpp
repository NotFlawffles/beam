#pragma once

#include "../../lexer/token.hpp"

namespace Beam::Text::Parser::Expression {
class Expression;

class Unary {
  public:
    Unary(const Lexer::Token::Type& _operator, Expression* expression)
        : _operator(_operator), expression(expression) {}

    Lexer::Token::Type getOperator() const { return _operator; }

    const Expression* getExpression() const { return expression; }

  private:
    const Lexer::Token::Type _operator;
    const Expression* expression;
};
} // namespace Beam::Text::Parser::Expression
