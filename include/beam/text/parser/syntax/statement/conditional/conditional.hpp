#pragma once

#include "../../primary/block.hpp"
#include "../../primary/expression.hpp"
#include "../statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Conditional {
class Conditional: public Statement {
  public:
    enum class Type { If, While, Do };

    Conditional(const Type& type, Primary::Expression* condition,
                Primary::Block* body)
        : Statement(Statement::Type::Conditional), conditionalType(type),
          condition(condition), body(body) {}

    Type getConditionalType() const { return conditionalType; }

    Primary::Expression* getCondition() const { return condition; }

    Primary::Block* getBody() const { return body; }

  private:
    const Type conditionalType;
    Primary::Expression* condition;
    Primary::Block* body;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Conditional
