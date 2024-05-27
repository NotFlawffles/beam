#include "../../../../../../../include/beam/text/parser/syntax/statement/conditional/if.hpp"

std::string Beam::Text::Parser::Syntax::Statement::Conditional::If::format() {
    return (getCondition() ? "if" : "else") +
           (getCondition() ? (' ' + getCondition()->format() + ' ') : " ") +
           getBody()->format() +
           (getElseClause()
                ? +(getElseClause()->getCondition() ? " else " : " ") +
                      getElseClause()->format()
                : "");
}

std::string Beam::Text::Parser::Syntax::Statement::Conditional::If::debug() {
    return "If(condition: " +
           (getCondition() ? getCondition()->debug() : "nullptr") +
           ", body: " + getBody()->debug() + ", elseClause: " +
           (getElseClause() ? getElseClause()->debug() : "nullptr");
}
