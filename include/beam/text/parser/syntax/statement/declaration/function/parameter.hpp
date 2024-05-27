#pragma once

#include "../../../annotation/declaration/type/type.hpp"
#include "../../declaration/declaration.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration::Function {
class Parameter: public Declaration {
  public:
    Parameter(Annotation::Declaration::Type::Type* type,
              Annotation::Declaration::Flags* flags, const std::string& name)
        : Declaration(Declaration::Type::FunctionParameter, type, flags, name) {
    }

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration::Function
