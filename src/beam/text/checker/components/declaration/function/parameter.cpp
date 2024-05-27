#include "../../../../../../../include/beam/text/checker/components/declaration/function/parameter.hpp"

std::string
Beam::Text::Checker::Components::Declaration::Function::Parameter::format() {
    return getFlags().format() + ' ' + getName() + ": " + getType()->format();
}

std::string
Beam::Text::Checker::Components::Declaration::Function::Parameter::debug() {
    return "Parameter(type: " + getType()->debug() +
           ", flags: " + getFlags().debug() + ", name: \"" + getName() + "\")";
}
