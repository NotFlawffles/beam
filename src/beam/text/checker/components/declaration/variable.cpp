#include "../../../../../../include/beam/text/checker/components/declaration/variable.hpp"

std::string Beam::Text::Checker::Components::Declaration::Variable::format() {
    return getFlags().format() + ' ' + getName() + ": " + getType()->format();
}

std::string Beam::Text::Checker::Components::Declaration::Variable::debug() {
    return "Variable(type: " + getType()->debug() +
           ", flags: " + getFlags().debug() + ", name: \"" + getName() + "\")";
}
