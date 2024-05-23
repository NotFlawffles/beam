#include "../../../../../../include/beam/text/parser/syntax/primary/expression.hpp"

std::string Beam::Text::Parser::Syntax::Primary::Expression::format() {
    return getValue()->format();
}

std::string Beam::Text::Parser::Syntax::Primary::Expression::debug() {
    return "Expression(value: " + getValue()->debug() + ')';
}
