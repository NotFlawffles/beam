#pragma once

#include "../../lexer/token.hpp"
#include "expression.hpp"
#include "operator.hpp"

namespace Beam::Text::Parser::Expression {
class Binary: public Expression {
  public:
    Binary(Expression* left, Operator* _operator, Expression* right)
        : Expression(Expression::Type::Binary), left(left), right(right),
          _operator(_operator) {}

    Expression* getLeft() const { return left; };

    Expression* getRight() const { return right; };

    Operator* getOperator() const { return _operator; }

    std::string format() override, debug() override;

  private:
    Expression *left, *right;
    Operator* _operator;
};
} // namespace Beam::Text::Parser::Expression
