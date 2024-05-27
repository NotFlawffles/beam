#pragma once

#include "../../primary/block.hpp"
#include "conditional.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class If: public Conditional {
  public:
    If(Primary::Expression* condition, Primary::Block* body, If* elseClause)
        : Conditional(Conditional::Type::If, condition, body),
          elseClause(elseClause) {}

    If* getElseClause() const { return elseClause; }

    std::string format() override, debug() override;

  private:
    If* elseClause;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
