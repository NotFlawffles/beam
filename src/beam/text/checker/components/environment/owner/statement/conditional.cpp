#include "../../../../../../../../include/beam/text/checker/components/environment/owner/statement/conditional.hpp"
#include "../../../../../../../../include/beam/compiler/utils/typechecking.hpp"

std::string Beam::Text::Checker::Components::Environment::Owner::Statement::
    Conditional::format() {
    auto current = getParent();

    while (Compiler::Utils::isDynamicallyCastableTo<Owner*, Conditional>(
        current)) {
        current = getParent();
    }

    return current->format();
}

std::string Beam::Text::Checker::Components::Environment::Owner::Statement::
    Conditional::debug() {
    auto current = getParent();

    while (Compiler::Utils::isDynamicallyCastableTo<Owner*, Conditional>(
        current)) {
        current = getParent();
    }

    return current->debug();
}
