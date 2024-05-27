#pragma once

#include "../../primary/block.hpp"
#include "../../primary/expression.hpp"
#include "conditional.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class While: public Conditional {
  public:
    While(Primary::Expression* condition, Primary::Block* body)
        : Conditional(Conditional::Type::While, condition, body) {}

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
