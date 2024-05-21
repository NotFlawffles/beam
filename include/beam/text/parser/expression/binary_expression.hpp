#pragma once

#include "../../lexer/token.hpp"

namespace Beam::Text::Parser::Expression {
class Expression;

class BinaryExpression {
  public:
    BinaryExpression(Expression* left, const Lexer::Token::Type& _operator,
                     Expression* right)
        : left(left), right(right), _operator(_operator) {}

    const Expression* getLeft() const { return left; }

    Lexer::Token::Type getOperator() const { return _operator; }

    const Expression* getRight() const { return right; }

  private:
    const Expression *left, *right;
    const Lexer::Token::Type _operator;
};
} // namespace Beam::Text::Parser::Expression
