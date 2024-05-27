#pragma once

#include <string>

#include "../../../../io/format/display.hpp"
#include "../../../parser/syntax/annotation/declaration/flags.hpp"
#include "../annotation/declaration/type/type.hpp"

namespace Beam::Text::Checker::Components::Declaration {
class Declaration: public IO::Format::Display {
  public:
    enum class DeclarationType { Variable, Function, FunctionParameter };

    Declaration(const DeclarationType& declarationType,
                Annotation::Declaration::Type::Type* type,
                const Parser::Syntax::Annotation::Declaration::Flags& flags,
                const std::string& name)
        : declarationType(declarationType), type(type), flags(flags),
          name(name) {}

    DeclarationType getDeclarationType() const { return declarationType; }

    Annotation::Declaration::Type::Type* getType() const { return type; }

    Parser::Syntax::Annotation::Declaration::Flags getFlags() const {
        return flags;
    }

    std::string getName() const { return name; }

  private:
    const DeclarationType declarationType;
    Annotation::Declaration::Type::Type* type;
    const Parser::Syntax::Annotation::Declaration::Flags flags;
    const std::string name;
};
} // namespace Beam::Text::Checker::Components::Declaration
