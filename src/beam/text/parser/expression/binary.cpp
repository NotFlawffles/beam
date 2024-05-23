#include "../../../../../include/beam/text/parser/expression/binary.hpp"
#include <memory>

std::string Beam::Text::Parser::Expression::Binary::format() {
    return getLeft()->format() + ' ' + getOperator()->format() + ' ' +
           getRight()->format();
}

std::string Beam::Text::Parser::Expression::Binary::debug() {
    return "Binary(left: " + getLeft()->debug() +
           ", _operator: " + getOperator()->debug() +
           ", right: " + getRight()->debug() + ')';
}
