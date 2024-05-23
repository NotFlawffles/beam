#pragma once

#include "../../primary/block.hpp"
#include "../../primary/expression.hpp"
#include "conditional.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class If: public Conditional {
  public:
    If(Primary::Expression* condition, Primary::Block* block, If* elseClause)
        : Conditional(Conditional::Type::If), condition(condition),
          block(block), elseClause(elseClause) {}

    Primary::Expression* getCondition() const { return condition; }

    Primary::Block* getBlock() const { return block; }

    If* getElseClause() const { return elseClause; }

    std::string format() override, debug() override;

  private:
    Primary::Expression* condition;
    Primary::Block* block;
    If* elseClause;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
