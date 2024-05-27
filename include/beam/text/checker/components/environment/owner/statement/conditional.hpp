#pragma once

#include "statement.hpp"

namespace Beam::Text::Checker::Components::Environment::Owner::Statement {
class Conditional: public Statement {
  public:
    Conditional(Owner* parent)
        : Statement(Statement::Type::Conditional), parent(parent) {}

    Owner* getParent() const { return parent; }

    std::string format() override, debug() override;

  private:
    Owner* parent;
};
} // namespace Beam::Text::Checker::Components::Environment::Owner::Statement
