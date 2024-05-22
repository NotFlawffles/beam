#pragma once

#include "../expression/expression.hpp"
#include "ast.hpp"

namespace Beam::Text::Parser::Syntax {
class Expression: public AbstractSyntaxTree {
  public:
    Expression(Parser::Expression::Expression* value)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Expression),
          value(value) {}

    Parser::Expression::Expression* getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    Parser::Expression::Expression* value;
};
} // namespace Beam::Text::Parser::Syntax
