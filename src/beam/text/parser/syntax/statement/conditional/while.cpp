#include "../../../../../../../include/beam/text/parser/syntax/statement/conditional/while.hpp"

std::string
Beam::Text::Parser::Syntax::Statement::Conditional::While::format() {
    return "while " + getCondition()->format() + ' ' + getBody()->format();
}

std::string Beam::Text::Parser::Syntax::Statement::Conditional::While::debug() {
    return "While(condition: " + getCondition()->debug() +
           ", body: " + getBody()->debug() + ')';
}
