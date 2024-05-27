#pragma once

#include "../../../../../../io/format/types/vector.hpp"
#include "../../../primary/block.hpp"
#include "../declaration.hpp"
#include "parameter.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration::Function {
class Function: public Declaration {
  public:
    Function(Annotation::Declaration::Type::Type* type,
             Annotation::Declaration::Flags* flags, const std::string& name,
             IO::Format::Types::Vector<Parameter*>* parameters,
             Primary::Block* body)
        : Declaration(Declaration::Type::Function, type, flags, name),
          parameters(parameters), body(body) {}

    IO::Format::Types::Vector<Parameter*>* getParameters() const {
        return parameters;
    }

    Primary::Block* getBody() const { return body; }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::Vector<Parameter*>* parameters;
    Primary::Block* body;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration::Function
