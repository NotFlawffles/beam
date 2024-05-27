#pragma once

#include "../../../../../io/format/types/vector.hpp"
#include "../declaration.hpp"
#include "parameter.hpp"

namespace Beam::Text::Checker::Components::Declaration::Function {
class Function: public Declaration {
  public:
    Function(Annotation::Declaration::Type::Type* type,
             const Parser::Syntax::Annotation::Declaration::Flags& flags,
             const std::string& name,
             IO::Format::Types::Vector<Parameter*>* parameters)
        : Declaration(DeclarationType::Function, type, flags, name),
          parameters(parameters) {}

    IO::Format::Types::Vector<Parameter*>* getParameters() const {
        return parameters;
    }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::Vector<Parameter*>* parameters;

    // TODO: complete Function
};
} // namespace Beam::Text::Checker::Components::Declaration::Function
