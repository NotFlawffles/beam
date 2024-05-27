#include "../../../../../../../include/beam/text/checker/components/declaration/function/function.hpp"

std::string
Beam::Text::Checker::Components::Declaration::Function::Function::format() {
    return getFlags().format() + ' ' + getName() + getParameters()->format() +
           ": " + getType()->format();
}

std::string
Beam::Text::Checker::Components::Declaration::Function::Function::debug() {
    return "Function(type: " + getType()->debug() +
           ", flags: " + getFlags().debug() + ", name: \"" + getName() +
           "\", parameters: " + getParameters()->debug() + ')';
}
