#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"

namespace Beam::Text::Parser::Expression {
class Binary: Expression {
  public:
    Binary(Expression* left, const Lexer::Token& _operator, Expression* right)
        : Expression(Expression::Type::Binary), left(left), right(right),
          _operator(_operator) {}

    Expression* getLeft() const { return left; };

    Expression* getRight() const { return right; };

    Lexer::Token getOperator() const { return _operator; }

    std::string format() override, debug() override;

  private:
    Expression *left, *right;
    Lexer::Token _operator;
};
} // namespace Beam::Text::Parser::Expression
