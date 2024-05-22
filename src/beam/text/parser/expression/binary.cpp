#include "../../../../../include/beam/text/parser/expression/binary.hpp"

std::string Beam::Text::Parser::Expression::Binary::format() { return debug(); }

std::string Beam::Text::Parser::Expression::Binary::debug() {
    return "Binary(left: " + getLeft()->debug() +
           ", _operator: " + getOperator().debug() +
           ", right: " + getRight()->debug() + ')';
}
