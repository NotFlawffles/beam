#pragma once

#include "../../primary/expression.hpp"
#include "declaration.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration {
class Variable: public Declaration {
  public:
    Variable(Anotation::Type::Type* anotationType, std::string name,
             Primary::Expression* value)
        : Declaration(Declaration::Type::Variable, anotationType, name),
          value(value) {}

    Primary::Expression* getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    Primary::Expression* value;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration
