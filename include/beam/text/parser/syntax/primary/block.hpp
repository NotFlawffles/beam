#pragma once

#include <memory>

#include "../../../../io/format/types/vector.hpp"
#include "primary.hpp"

namespace Beam::Text::Parser::Syntax::Primary {
class Block: public Primary {
  public:
    Block(const IO::Format::Types::Vector<AbstractSyntaxTree*>& value)
        : Primary(Primary::Type::Block), value(value) {}

    IO::Format::Types::Vector<AbstractSyntaxTree*> getValue() const {
        return value;
    }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::Vector<AbstractSyntaxTree*> value;
};
} // namespace Beam::Text::Parser::Syntax::Primary
