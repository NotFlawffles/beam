#include "../../../../../include/beam/text/parser/syntax/expression.hpp"

std::string Beam::Text::Parser::Syntax::Expression::format() {
    return getValue()->format();
}

std::string Beam::Text::Parser::Syntax::Expression::debug() {
    return "Expression(value: " + getValue()->debug() + ')';
}
