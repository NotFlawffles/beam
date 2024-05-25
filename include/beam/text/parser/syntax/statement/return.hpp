#pragma once

#include "../primary/expression.hpp"
#include "statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement {
class Return: public Statement {
  public:
    Return(Primary::Expression* expression)
        : Statement(Statement::Type::Return), expression(expression) {}

    Primary::Expression* getExpression() const { return expression; }

    std::string format() override, debug() override;

  private:
    Primary::Expression* expression;
};
} // namespace Beam::Text::Parser::Syntax::Statement
