#pragma once

#include "../../../declaration/declaration.hpp"
#include "statement.hpp"

namespace Beam::Text::Checker::Components::Environment::Owner::Statement {
class Declaration: public Statement {
  public:
    explicit Declaration(Components::Declaration::Declaration* declaration)
        : Statement(Statement::Type::Declaration), declaration(declaration) {}

    Components::Declaration::Declaration* getDeclaration() const {
        return declaration;
    }

    std::string format() override, debug() override;

  private:
    Components::Declaration::Declaration* declaration;
};
} // namespace Beam::Text::Checker::Components::Environment::Owner::Statement
