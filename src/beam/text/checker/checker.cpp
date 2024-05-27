#include "../../../../include/beam/text/checker/checker.hpp"

std::string Beam::Text::Checker::Checker::format() {
    return getRoot()->format();
}

std::string Beam::Text::Checker::Checker::debug() {
    return "Checker(root: " + getRoot()->debug() + ')';
}
