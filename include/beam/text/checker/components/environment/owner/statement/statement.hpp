#pragma once

#include "../owner.hpp"

namespace Beam::Text::Checker::Components::Environment::Owner::Statement {
class Statement: public Owner {
  public:
    enum class Type { Conditional, Declaration };

    explicit Statement(const Type& type)
        : Owner(Owner::Type::Statement), statementType(type) {}

    Type getStatementType() const { return statementType; }

  private:
    const Type statementType;
};
} // namespace Beam::Text::Checker::Components::Environment::Owner::Statement
