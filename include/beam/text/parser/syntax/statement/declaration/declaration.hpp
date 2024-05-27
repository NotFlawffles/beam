#pragma once

#include "../../../../../io/format/types/char.hpp"
#include "../../annotation/declaration/flags.hpp"
#include "../../annotation/declaration/type/type.hpp"
#include "../statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration {
class Declaration: public Statement {
  public:
    enum class Type { Variable, Function, FunctionParameter };

    explicit Declaration(const Type& declarationType,
                         Annotation::Declaration::Type::Type* anotationType,
                         Annotation::Declaration::Flags* flags,
                         std::string name)
        : Statement(Statement::Type::Declaration),
          declarationType(declarationType), anotationType(anotationType),
          flags(flags), name(name) {}

    Type getDeclarationType() const { return declarationType; }

    Annotation::Declaration::Type::Type* getAnotationType() const {
        return anotationType;
    }

    Annotation::Declaration::Flags* getFlags() const { return flags; }

    std::string getName() const { return name; }

  private:
    const Type declarationType;
    Annotation::Declaration::Type::Type* anotationType;
    Annotation::Declaration::Flags* flags;
    const std::string name;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration
