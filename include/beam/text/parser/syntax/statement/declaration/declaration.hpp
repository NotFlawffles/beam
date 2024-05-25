#pragma once

#include "../../anotation/type/type.hpp"
#include "../statement.hpp"

namespace Beam::Text::Parser::Syntax::Statement::Declaration {
class Declaration: public Statement {
  public:
    enum class Type { Variable, Function };

    explicit Declaration(const Type& declarationType,
                         Anotation::Type::Type* anotationType, std::string name)
        : Statement(Statement::Type::Declaration),
          declarationType(declarationType), anotationType(anotationType),
          name(name) {}

    Type getDeclarationType() const { return declarationType; }

    Anotation::Type::Type* getAnotationType() const { return anotationType; }

    std::string getName() const { return name; }

  private:
    const Type declarationType;
    Anotation::Type::Type* anotationType;
    const std::string name;
};
} // namespace Beam::Text::Parser::Syntax::Statement::Declaration
