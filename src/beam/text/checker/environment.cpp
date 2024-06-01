#include <algorithm>

#include "../../../../include/beam/diagnostic/warning.hpp"
#include "../../../../include/beam/text/checker/environment.hpp"

Beam::Text::Parser::Syntax::Statement::Declaration::Declaration*
Beam::Text::Checker::Environment::get(const std::string& name,
                                      const bool& lookInParents) const {
    Parser::Syntax::Statement::Declaration::Declaration* match = nullptr;

    for (const auto& declaration : getDeclarations()) {
        if (declaration->getName() == name) {
            match = declaration;
        }
    }

    if (!match) {
        if (lookInParents) {
            if (hasParent()) {
                return getParent()->get(name, lookInParents);
            }
        }
    }

    return match;
}

bool Beam::Text::Checker::Environment::add(
    Beam::Text::Parser::Syntax::Statement::Declaration::Declaration*
        declaration) {
    if (exists(declaration->getName())) {
        return false;
    }

    declarations.push_back(declaration);
    return true;
}

bool Beam::Text::Checker::Environment::exists(const std::string& name,
                                              const bool& lookInParents) const {
    auto found = false;

    for (const auto& declaration : getDeclarations()) {
        if (declaration->getName() == name) {
            found = true;
        }
    }

    if (!found) {
        if (lookInParents) {
            if (hasParent()) {
                return getParent()->exists(name, lookInParents);
            }
        }
    }

    return found;
}
