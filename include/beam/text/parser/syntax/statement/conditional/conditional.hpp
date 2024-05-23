#pragma once

#include "../statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class Conditional: public Statement {
  public:
    enum class Type { If, While };

    Conditional(const Type& type)
        : Statement(Statement::Type::Conditional), conditionalType(type) {}

    Type getConditionalType() const { return conditionalType; }

  private:
    const Type conditionalType;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
