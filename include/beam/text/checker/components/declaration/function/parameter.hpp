#pragma once

#include "../declaration.hpp"

namespace Beam::Text::Checker::Components::Declaration::Function {
class Parameter: public Declaration {
  public:
    Parameter(Annotation::Declaration::Type::Type* type,
              const Parser::Syntax::Annotation::Declaration::Flags& flags,
              const std::string& name)
        : Declaration(DeclarationType::FunctionParameter, type, flags, name) {}

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Checker::Components::Declaration::Function
