#pragma once

#include "../../io/format/types/vector.hpp"
#include "../parser/syntax/statement/declaration/declaration.hpp"

namespace Beam::Text::Checker {
class Environment {
  public:
    explicit Environment(Environment* parent)
        : parent(parent), declarations({}) {}

    Parser::Syntax::Statement::Declaration::Declaration*
    get(const std::string& name, const bool& lookInParents = true) const;

    bool add(Parser::Syntax::Statement::Declaration::Declaration* declaration),
        exists(const std::string& name, const bool& lookInParents = true) const;

    Environment* getParent() const { return parent; }

    IO::Format::Types::Vector<
        Parser::Syntax::Statement::Declaration::Declaration*>
    getDeclarations() const {
        return declarations;
    }

    /* just for code convension and "readability" */
    bool hasParent() const { return parent; }

  private:
    Environment* parent;

    IO::Format::Types::Vector<
        Parser::Syntax::Statement::Declaration::Declaration*>
        declarations;
};
} // namespace Beam::Text::Checker
