#pragma once

#include "../../../../../io/format/types/char.hpp"
#include "../../primary/expression.hpp"
#include "declaration.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration {
class Variable: public Declaration {
  public:
    Variable(Annotation::Declaration::Type::Type* anotationType,
             Annotation::Declaration::Flags* flags, const std::string& name,
             Primary::Expression* value)
        : Declaration(Declaration::Type::Variable, anotationType, flags, name),
          value(value) {}

    Primary::Expression* getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    Primary::Expression* value;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration
