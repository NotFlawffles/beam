#include "../../../../../../include/beam/text/parser/syntax/statement/return.hpp"

std::string Beam::Text::Parser::Syntax::Statement::Return::format() {
    return "return " + getExpression()->format();
}

std::string Beam::Text::Parser::Syntax::Statement::Return::debug() {
    return "Return(expression: " + getExpression()->debug() + ')';
}
