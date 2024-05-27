#include "../../../../../../../../include/beam/text/parser/syntax/statement/declaration/function/function.hpp"

std::string Beam::Text::Parser::Syntax::Statement::Declaration::Function::
    Function::format() {
    return "let " + getFlags()->format() + ' ' + getName() + "(" +
           getParameters()->formatNoBraces() +
           "): " + getAnotationType()->format() + ' ' + getBody()->format();
}

std::string Beam::Text::Parser::Syntax::Statement::Declaration::Function::
    Function::debug() {
    return "Function(anotationType: " + getAnotationType()->debug() +
           ", flags: " + getFlags()->debug() + ", name: " + getName() +
           ", parameters: " + getParameters()->debug() +
           ", body: " + getBody()->debug() + ')';
}
