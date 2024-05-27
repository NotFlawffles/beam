#include <algorithm>

#include "../../../../../../include/beam/diagnostic/warning.hpp"
#include "../../../../../../include/beam/text/checker/components/environment/environment.hpp"

Beam::Diagnostic::DiResult<
    Beam::Text::Checker::Components::Declaration::Declaration*,
    Beam::Diagnostic::Diagnostic*>
Beam::Text::Checker::Components::Environment::Environment::get(
    const std::string& name, const bool& lookForParents) {
    Declaration::Declaration* match;

    for (const auto& declaration : *getDeclarations()) {
        if (declaration->getName() == name) {
            match = declaration;
        }
    }

    if (!match) {
        if (getParent() && lookForParents) {
            return getParent()->get(name);
        } else {
            return new Diagnostic::Warning(
                Diagnostic::Warning::Type::WarningTypeUndeclaredName,
                Diagnostic::Warning::Icon::WarningIconName, span->clone(),
                "name `" + name + "` is undeclared.");
        }
    }

    return match;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Checker::Components::Declaration::Declaration*,
    Beam::Diagnostic::Diagnostic*>
Beam::Text::Checker::Components::Environment::Environment::get(
    Beam::Text::Checker::Components::Declaration::Declaration* declaration,
    const bool& lookForParents) {
    Declaration::Declaration* match;

    for (const auto& current : *getDeclarations()) {
        if (current == declaration) {
            match = current;
        }
    }

    if (!match) {
        if (getParent() && lookForParents) {
            return getParent()->get(declaration);
        } else {
            return new Diagnostic::Warning(
                Diagnostic::Warning::Type::WarningTypeUndeclaredName,
                Diagnostic::Warning::Icon::WarningIconName, span->clone(),
                "name `" + declaration->getName() + "` is undeclared.");
        }
    }

    return match;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Checker::Components::Declaration::Declaration*,
    Beam::Diagnostic::Diagnostic*>
Beam::Text::Checker::Components::Environment::Environment::add(
    Beam::Text::Checker::Components::Declaration::Declaration* declaration) {
    if (get(declaration, false).isSuccess()) {
        return new Diagnostic::Warning(
            Diagnostic::Warning::Type::WarningTypeUndeclaredName,
            Diagnostic::Warning::Icon::WarningIconName, span->clone(),
            "cannot redeclare name `" + declaration->getName() + "`.");
    }

    getDeclarations()->push_back(declaration);
    return declaration;
}

std::string
Beam::Text::Checker::Components::Environment::Environment::format() {
    return getOwner()->format() + ": " + getDeclarations()->format();
}

std::string Beam::Text::Checker::Components::Environment::Environment::debug() {
    return "Environment(owner: " + getOwner()->debug() +
           ", declarations: " + getDeclarations()->debug() + ')';
}
