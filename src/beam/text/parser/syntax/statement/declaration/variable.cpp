#include "../../../../../../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"

std::string
Beam::Text::Parser::Syntax::Statement::Declaration::Variable::format() {
    return "let " + getName() + ": " + getAnotationType()->format() + " = " +
           getValue()->format();
}

std::string
Beam::Text::Parser::Syntax::Statement::Declaration::Variable::debug() {
    return "Variable(anotationType: " + getAnotationType()->debug() +
           ", name: " + getName() + ", value: " + getValue()->debug() + ')';
}
