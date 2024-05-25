#pragma once

#include "../ast.hpp"

namespace Beam::Text::Parser::Syntax::Statement {
class Statement: public AbstractSyntaxTree {
  public:
    enum class Type { Conditional, Return, Declaration };

    Statement(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Statement),
          statementType(type) {}

    Type getStatementType() const { return statementType; }

  private:
    const Type statementType;
};
} // namespace Beam::Text::Parser::Syntax::Statement
