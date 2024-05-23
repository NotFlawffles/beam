#pragma once

#include "../../expression/expression.hpp"
#include "primary.hpp"

namespace Beam::Text::Parser::Syntax::Primary {
class Expression: public Primary {
  public:
    Expression(Beam::Text::Parser::Expression::Expression* value)
        : Primary(Primary::Type::Expression), value(value) {}

    Beam::Text::Parser::Expression::Expression* getValue() const {
        return value;
    }

    std::string format() override, debug() override;

  private:
    Beam::Text::Parser::Expression::Expression* value;
};
} // namespace Beam::Text::Parser::Syntax::Primary
