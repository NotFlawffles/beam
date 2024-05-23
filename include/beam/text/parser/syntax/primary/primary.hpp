#pragma once

#include "../ast.hpp"

namespace Beam::Text::Parser::Syntax::Primary {
class Primary: public AbstractSyntaxTree {
  public:
    enum class Type { Block, Expression };

    Primary(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Primary),
          primaryType(type) {}

    Type getPrimaryType() const { return primaryType; }

  private:
    const Type primaryType;
};
} // namespace Beam::Text::Parser::Syntax::Primary
