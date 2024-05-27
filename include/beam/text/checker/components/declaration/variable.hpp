#pragma once

#include "declaration.hpp"

namespace Beam::Text::Checker::Components::Declaration {
class Variable: public Declaration {
  public:
    Variable(Annotation::Declaration::Type::Type* type,
             const Parser::Syntax::Annotation::Declaration::Flags& flags,
             const std::string& name)
        : Declaration(Declaration::DeclarationType::Variable, type, flags,
                      name) {}

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Checker::Components::Declaration
