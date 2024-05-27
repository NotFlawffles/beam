#pragma once

#include "conditional.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class Do: public Conditional {
  public:
    Do(Primary::Expression* condition, Primary::Block* body)
        : Conditional(Conditional::Type::Do, condition, body) {}

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
