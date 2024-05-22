#include "../../../../../include/beam/text/parser/expression/unary.hpp"

std::string Beam::Text::Parser::Expression::Unary::format() { return debug(); }

std::string Beam::Text::Parser::Expression::Unary::debug() {
    return "Unary(expression: " + getExpression()->debug() +
           ", operator: " + getOperator().debug() + ')';
}
