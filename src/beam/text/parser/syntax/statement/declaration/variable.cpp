#include "../../../../../../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"

std::string
Beam::Text::Parser::Syntax::Statement::Declaration::Variable::format() {
    return "let " + getFlags()->format() + ' ' + getName() + ": " +
           getAnotationType()->format() + " = " + getValue()->format();
}

std::string
Beam::Text::Parser::Syntax::Statement::Declaration::Variable::debug() {
    return "Variable(anotationType: " + getAnotationType()->debug() +
           ", flags: " + getFlags()->debug() + ", name: " + getName() +
           ", value: " + getValue()->debug() + ')';
}
