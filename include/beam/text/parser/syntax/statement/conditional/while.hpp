#pragma once

#include "../../primary/block.hpp"
#include "../../primary/expression.hpp"
#include "conditional.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class While: public Conditional {
  public:
    While(Primary::Expression* condition, Primary::Block* block)
        : Conditional(Conditional::Type::While, condition), block(block) {}

    Primary::Block* getBlock() const { return block; }

    std::string format() override, debug() override;

  private:
    Primary::Block* block;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
