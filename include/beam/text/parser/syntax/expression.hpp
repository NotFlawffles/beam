#pragma once

#include "../expression/expression.hpp"
#include "ast.hpp"

namespace Beam::Text::Parser::Syntax {
class Expression: public AbstractSyntaxTree {
  public:
    Expression(Beam::Text::Parser::Expression::Expression* value)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Expression),
          value(value) {}

    Beam::Text::Parser::Expression::Expression* getValue() const {
        return value;
    }

    std::string format() override, debug() override;

  private:
    Beam::Text::Parser::Expression::Expression* value;
};
} // namespace Beam::Text::Parser::Syntax
