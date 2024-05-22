#pragma once

#include <memory>

#include "../../../io/format/types/vector.hpp"
#include "ast.hpp"

namespace Beam::Text::Parser::Syntax {
class Block: public AbstractSyntaxTree {
  public:
    Block(const IO::Format::Types::Vector<AbstractSyntaxTree*>& value)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Block), value(value) {}

    IO::Format::Types::Vector<AbstractSyntaxTree*> getValue() const {
        return value;
    }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::Vector<AbstractSyntaxTree*> value;
};
} // namespace Beam::Text::Parser::Syntax
