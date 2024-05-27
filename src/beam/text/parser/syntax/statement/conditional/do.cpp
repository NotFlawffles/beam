#include "../../../../../../../include/beam/text/parser/syntax/statement/conditional/do.hpp"

std::string Beam::Text::Parser::Syntax::Statement::Conditional::Do::format() {
    return "do " + getBody()->format() + " while " + getCondition()->format();
}

std::string Beam::Text::Parser::Syntax::Statement::Conditional::Do::debug() {
    return "Do(condition: " + getCondition()->debug() +
           ", body: " + getBody()->debug() + ')';
}
