#pragma once

#include "../../primary/expression.hpp"
#include "../statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class Conditional: public Statement {
  public:
    enum class Type { If, While };

    Conditional(const Type& type, Primary::Expression* condition)
        : Statement(Statement::Type::Conditional), conditionalType(type),
          condition(condition) {}

    Type getConditionalType() const { return conditionalType; }

    Primary::Expression* getCondition() const { return condition; }

  private:
    const Type conditionalType;
    Primary::Expression* condition;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
