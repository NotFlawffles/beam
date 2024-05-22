#pragma once

#include <memory>

#include "../../../io/format/types/vector.hpp"
#include "ast.hpp"

namespace Beam::Text::Parser::Syntax {
class Block: AbstractSyntaxTree {
  public:
    Block(const Debug::Types::Vector<AbstractSyntaxTree*>& value)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Block), value(value) {}

    Debug::Types::Vector<AbstractSyntaxTree*> getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    Debug::Types::Vector<AbstractSyntaxTree*> value;
};
} // namespace Beam::Text::Parser::Syntax
